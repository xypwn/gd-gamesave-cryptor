#include <iostream>
#include "GDGameSave.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	// Using Windows
	const std::string gdLocalFolder = getenv("APPDATA") + std::string("\\..\\Local\\GeometryDash\\");
#else
	// Using GNU/Linux
	const std::string gdLocalFolder = getenv("HOME") + std::string("/.local/share/Steam/steamapps/compatdata/322170/pfx/drive_c/users/steamuser/Local Settings/Application Data/GeometryDash/");
#endif

const std::string usage = R"(Usage:
	-d: decrypt
	-e: encrypt
	-es: encrypt and save to GD folder)";

const std::string saves[2] = {
	"CCGameManager.dat",
	"CCLocalLevels.dat",
};

int main(int argc, const char** argv) {
	std::vector<std::string> args;
	for(int i = 0; i < argc; i++)
		args.push_back(argv[i]);

	if(argc < 2)
	{
		std::cout << usage << std::endl;
		exit(1);
	}

	if(args[1] == "-d")
	{
		const std::string decrypted = Decrypt(gdLocalFolder + saves[0]);
		StrToFile(saves[0] + ".xml", decrypted);
	}
	else if(args[1] == "-e" || args[1] == "-es")
	{
		const std::string encrypted = Encrypt(saves[0] + ".xml");
		if(args[1] == "-e")
			StrToFile(saves[0], encrypted);
		else
			StrToFile(gdLocalFolder + saves[0], encrypted);
	}

	exit(0);
}