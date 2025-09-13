// texteditor.hpp
/* TextEditor created by HassanIQ777
created: 2025-Jun-27
updated: 2025-Jul-9
*/

#ifndef TEXTEDITOR
#define TEXTEDITOR

#include "libutils/src/file.hpp"
#include "libutils/src/funcs.hpp"
#include <iomanip>

/* TODO: 
 * Add better status bar, file name, lines count (10/150), current time
 * like MediaPlayer, make pages
 * allow page skipping with PgUp, PgDn
 * add f(ind) to find text AND move p_current_line to it
 * add g(oto beginning) and G(oto end)
 * add a void showFileData() that shows info like file size, creation date, last modification
 */

// start of class
class TextEditor
{
  public:
	TextEditor(std::string filename)
		: p_filename(filename)
	{
		p_file_content = File::m_readfile(p_filename);
	}

	enum class m_Mode
	{
		MODE_VIEW_ONLY,
		MODE_EDIT
	};

	void m_openEditor();
	void m_setMode(const m_Mode &mode);

  private:
	// p for private
	m_Mode p_editor_mode = m_Mode::MODE_EDIT;
	std::string p_filename;
	std::vector<std::string> p_file_content;
	int p_current_line = 0;
	bool p_soft_overwrap = false;

	std::string p_trimStr(std::string text, int WIDTH);
	std::string p_getModeString();
	void p_showHelp();
};

void TextEditor::m_openEditor()
{
	bool running = true;

	while (running)
	{
		funcs::clearTerminal();

		int size = p_file_content.size();
		int num_digits = std::to_string(size).size();
		int WIDTH = funcs::getTerminalWidth();

		for (int i = 0; i < WIDTH; i++)
		{
			funcs::print("_");
		}
		funcs::print("\n");

		for (int i = 0; i < size; i++)
		{
			std::string line;
			if (!p_soft_overwrap)
			{
				line = p_trimStr(p_file_content[i], WIDTH - 2 - num_digits);
			}
			else
			{
				line = p_file_content[i];
			}
			if (i == p_current_line)
			{
				std::cout << "\x1b[1m"
						  << "\x1b[33m" << std::setw(num_digits) << std::right << i + 1 << "│ "
						  << "\x1b[0m" << line
						  << "\n";
			}
			else
			{
				std::cout << "\x1b[38;2;110;110;110m" << std::setw(num_digits) << std::right << i + 1 << "│ "
						  << "\x1b[0m" << line
						  << "\n";
			}
		}

		for (int i = 0; i < WIDTH; i++)
		{
			funcs::print("─");
		}
		funcs::print("\n");
		funcs::print("Mode: ", p_getModeString(), "\n");
		funcs::print("'?' for help\n");

		std::string input;
		input = funcs::getKeyPress();

		std::string line_number_str;

		if (input == "i" && p_editor_mode > m_Mode::MODE_VIEW_ONLY)
		{
			funcs::print("Insert: ");
			std::string line;
			std::getline(std::cin, line);
			File::m_writeline(p_filename, line, p_current_line);
		}
		else if (input == "r" && p_editor_mode > m_Mode::MODE_VIEW_ONLY)
		{
			if (size == 0)
			{
				continue;
			}
			std::string removed_line = p_file_content[p_current_line];
			if (File::m_removeline(p_filename, p_current_line))
			{
				funcs::print("removed line(", p_current_line + 1, ") '", removed_line, "'\n");
			}
			else
			{
				funcs::print("Failed to remove line(", p_current_line + 1, ")\n");
			}

			if (p_current_line > 0)
			{
				p_current_line--;
			}
			if (removed_line != "")
			{
				funcs::getKeyPress();
			}
		}
		else if (input == "?")
		{
			p_showHelp();
		}
		else if (input == "l" && p_editor_mode > m_Mode::MODE_VIEW_ONLY)
		{
			std::cout << "Line number: ";
			std::cin >> line_number_str;
			std::cin.ignore();
			std::cin.clear();
			if (!funcs::isNumber(line_number_str))
			{
				continue;
			}
			int temp = std::stoi(line_number_str);

			if (temp > size)
			{
				p_current_line = size - 1;
			}
			else if (temp < 1)
			{
				p_current_line = 0;
			}
		}
		else if ((input == " " || input == "n" || input == "\033[A") && p_editor_mode > m_Mode::MODE_VIEW_ONLY)
		{
			if (p_current_line < size - 1)
			{
				p_current_line++;
			}
		}
		else if ((input[0] == char(127) || input == "b" || input == "\033[B") && p_editor_mode > m_Mode::MODE_VIEW_ONLY)
		{
			if (p_current_line > 0)
			{
				p_current_line--;
			}
		}
		else if ((input[0] == char(10) || input == "a") && p_editor_mode > m_Mode::MODE_VIEW_ONLY)
		{
			p_file_content.insert(p_file_content.begin() + (++p_current_line), "");
			File::m_writefile(p_filename, p_file_content);
		}
		else if (input == "s")
		{
			p_soft_overwrap = !p_soft_overwrap;
		}
		else if (input == "q")
		{
			running = false;
		}
		else
		{
			continue;
		}

		p_file_content = File::m_readfile(p_filename);
	}
}

std::string TextEditor::p_trimStr(std::string text, int WIDTH)
{
	if (text.length() <= WIDTH)
	{
		return text;
	}
	return text.substr(0, WIDTH - 2) + "··";
}

void TextEditor::p_showHelp()
{
	funcs::print("i(nsert line)\n");
	funcs::print("r(emove line)\n");
	funcs::print("l(ine number) = (", p_current_line + 1, ")\n");
	funcs::print("n(ext line)\n");
	funcs::print("b(ack a line)\n");
	funcs::print("a(dd line)\n");
	funcs::print("s(oft line overwrapping) = ", std::boolalpha, p_soft_overwrap, "\n");
	funcs::print("q(uit)\n");
	funcs::getKeyPress();
}

void TextEditor::m_setMode(const m_Mode &mode)
{
	p_editor_mode = mode;
}

std::string TextEditor::p_getModeString()
{
	std::string mode;
	switch (p_editor_mode)
	{
	case m_Mode::MODE_VIEW_ONLY:
		mode = "View Only Mode";
		break;

	case m_Mode::MODE_EDIT:
		mode = "Edit Mode";
		break;

	default:
		mode = "???"; // how??
		break;
	}
	return mode;
}
// end of class

#endif