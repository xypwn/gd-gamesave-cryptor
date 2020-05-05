#include <iostream>
#include "GDGameSave.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	// Using Windows
	const std::string gdLocalFolder = getenv("APPDATA") + std::string("\\..\\Local\\GeometryDash\\");
#else
	// Using GNU/Linux
	const std::string gdLocalFolder = getenv("HOME") + std::string("/.local/share/Steam/steamapps/compatdata/322170/pfx/drive_c/users/steamuser/Local Settings/Application Data/GeometryDash/");
#endif

const std::string saves[2] = {
	"CCGameManager.dat",
	"CCLocalLevels.dat",
};

const std::string commands = R"(Commands:
    1: Decrypt
    2: Encrypt
    3: Encrypt and save to GD folder
    0: Quit)";

enum class Command
{
	Decrypt,
	Encrypt
};

bool ExecCommand(Command cmd, const std::string& in_filename, const std::string& out_filename)
{
	if(!IsFile(in_filename))
	{
		std::cerr << "ERROR: Could not open file " << in_filename << " for reading" << std::endl;
		return false;
	}
	switch(cmd)
	{
	case Command::Decrypt:
		std::cout << "Decrypting " << in_filename << "..." << std::endl;
		if(!StrToFile(out_filename, Decrypt(in_filename)))
		{
			std::cerr << "ERROR: Could not open file " << out_filename << " for writing" << std::endl;
			return false;
		}
		break;
	case Command::Encrypt:
		std::cout << "Encrypting " << in_filename << "..." << std::endl;
		if(!StrToFile(out_filename, Encrypt(in_filename)))
		{
			std::cerr << "ERROR: Could not open file " << out_filename << " for writing" << std::endl;
			return false;
		}
	}
	return true;
}

int main(int argc, const char** argv) {
	std::vector<std::string> args;
	for(int i = 0; i < argc; i++)
		args.push_back(argv[i]);

	std::string cmd;

	while(true)
	{
		std::cout << commands << std::endl;
		std::cout << ">> ";
		std::cin >> cmd;
		if(cmd == "1")
		{
			for(unsigned int i = 0; i < 2; i++)
			{
				if(!ExecCommand(Command::Decrypt, gdLocalFolder + saves[i], saves[i] + ".xml"))
					continue;
			}
		}
		else if(cmd == "2")
		{
			for(unsigned int i = 0; i < 2; i++)
			{
				if(!ExecCommand(Command::Encrypt, saves[i] + ".xml", saves[i]))
					continue;
			}
		}
		else if(cmd == "3")
		{
			for(unsigned int i = 0; i < 2; i++)
			{
				if(!ExecCommand(Command::Encrypt, saves[i] + ".xml", gdLocalFolder + saves[i]))
					continue;
			}
		}
		else if(cmd == "0")
		{
			std::cout << "Quitting" << std::endl;
			break;
		}
		else
			std::cerr << "Invalid command" << std::endl;
	}

	return(0);
}