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

int main(int argc, const char** argv) {
	std::vector<std::string> args;
	for(int i = 0; i < argc; i++)
		args.push_back(argv[i]);

	bool quit = false;
	std::string cmd;

	while(!quit)
	{
		std::cout << commands << std::endl;
		std::cout << ">> ";
		std::cin >> cmd;
		if(cmd == "1")
		{
			std::cout << "Decrypting..." << std::endl;
			const std::string decrypted = Decrypt(gdLocalFolder + saves[0]);
			StrToFile(saves[0] + ".xml", decrypted);
		}
		else if(cmd == "2")
		{
			std::cout << "Encrypting..." << std::endl;
			const std::string encrypted = Encrypt(saves[0] + ".xml");
			StrToFile(saves[0], encrypted);
		}
		else if(cmd == "3")
		{
			std::cout << "Encrypting and saving to GD folder..." << std::endl;
			const std::string encrypted = Encrypt(saves[0] + ".xml");
			StrToFile(gdLocalFolder + saves[0], encrypted);
		}
		else if(cmd == "0")
		{
			std::cout << "Quitting" << std::endl;
			quit = true;
		}
		else
			std::cerr << "Invalid command" << std::endl;
	}

	return(0);
}