/*
license:
	All rights reserved to HassanIQ777
	You may:
		Use the code below, edit it or change it however you like, 
		but never republish it under a new name, 
		if so you may do it while crediting me.

	@ use this to create tables, feel free to change it to customize your tables.
Made on 		  2025-Jul-15
Last update as of 2025-Jul-15
*/

#ifndef TABLE_HPP
#define TABLE_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

/* EXAMPLE: */
/*
Table table;

table.m_setHeader("Name", "Score", "Grade");
table.m_addRow("Hassan", 99, "S+");
	
std::cout << table;

table.m_exportCSV("output.csv");
*/

class Table
{
  private:
	std::vector<std::vector<std::string>> rows;
	std::vector<size_t> colWidths;
	int trailing_spaces = 2;
	char filler_char = '-';

	template <typename T>
	std::string toString(const T &value) const
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}

	std::string copyString(const int &n, const std::string &char_) const
	{
		std::string ret;

		for (int i = 0; i < n; i++)
		{
			ret += char_;
		}

		return ret;
	}

	void updateColWidths()
	{
		colWidths.clear();
		for (const auto &row : rows)
		{
			if (row.size() > colWidths.size())
				colWidths.resize(row.size(), 0);
			for (size_t i = 0; i < row.size(); ++i)
			{
				colWidths[i] = std::max(colWidths[i], row[i].length());
			}
		}
	}

  public:
	template <typename... Args>
	void m_addRow(Args &&... args)
	{
		std::vector<std::string> row = {toString(args)...};
		rows.push_back(std::move(row));
		updateColWidths();
	}

	template <typename... Args>
	void m_setHeader(Args &&... args)
	{
		rows.insert(rows.begin(), {toString(args)...});
		updateColWidths();
	}

	void m_setTrailingSpaces(int trailing_spaces_)
	{
		trailing_spaces = trailing_spaces_;
	}

	void m_setFillerChar(char ch)
	{
		filler_char = ch;
	}

	// The CSV Export Method
	void m_exportCSV(const std::string &filename) const
	{
		std::ofstream file(filename);
		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << filename << "\n";
			return;
		}

		for (const auto &row : rows)
		{
			for (size_t i = 0; i < row.size(); ++i)
			{
				std::string value = row[i];

				// Escape quotes inside cells by doubling them
				size_t pos = 0;
				while ((pos = value.find('"', pos)) != std::string::npos)
				{
					value.insert(pos, "\"");
					pos += 2;
				}

				// Wrap each cell in quotes (Excel-friendly)
				file << "\"" << value << "\"";
				if (i < row.size() - 1)
					file << ",";
			}
			file << "\n";
		}

		file.close();
		//std::cout << "Exported to: " << filename << "\n";
	}

	void m_importCSV(const std::string &filename)
	{
		std::ifstream file(filename);
		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << filename << "\n";
			return;
		}

		rows.clear();
		std::string line;
		while (std::getline(file, line))
		{
			std::vector<std::string> row;
			std::string cell;
			bool insideQuotes = false;

			for (size_t i = 0; i < line.size(); ++i)
			{
				char c = line[i];

				if (c == '"')
				{
					// Handle escaped quotes ("")
					if (insideQuotes && i + 1 < line.size() && line[i + 1] == '"')
					{
						cell += '"';
						++i; // skip the second quote
					}
					else
					{
						insideQuotes = !insideQuotes; // toggle state
					}
				}
				else if (c == ',' && !insideQuotes)
				{
					row.push_back(cell);
					cell.clear();
				}
				else
				{
					cell += c;
				}
			}
			row.push_back(cell);

			rows.push_back(std::move(row));
		}

		file.close();
		updateColWidths();
		//std::cout << "Imported from: " << filename << "\n";
	}

	// Text output
	friend std::ostream &operator<<(std::ostream &os, const Table &table)
	{
		for (size_t rowIndex = 0; rowIndex < table.rows.size(); ++rowIndex)
		{
			const auto &row = table.rows[rowIndex];
			for (size_t i = 0; i < row.size(); ++i)
			{
				os << std::left << std::setw(table.colWidths[i] + table.trailing_spaces)
				   << row[i];
			}
			os << "\n";

			if (rowIndex == 0)
			{
				for (size_t i = 0; i < row.size(); ++i)
				{
					os << std::string(table.colWidths[i], table.filler_char);
					os << std::string(table.trailing_spaces, ' ');
				}
				os << "\n";
			}
		}
		return os;
	}
};

#endif // table.hpp