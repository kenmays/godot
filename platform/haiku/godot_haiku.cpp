#include "os_haiku.h"

#include "main/main.h"

int main(int argc, char *argv[]) {
	OS_Haiku os;
	Error err = Main::setup(argv[0], argc - 1, &argv[1]);
	if (err != OK) {
		return err == ERR_HELP ? EXIT_SUCCESS : EXIT_FAILURE;
	}
	if (Main::start() != EXIT_SUCCESS) {
		os.set_exit_code(EXIT_FAILURE);
		Main::cleanup();
		return os.get_exit_code();
	}
	os.run();
	Main::cleanup();
	return os.get_exit_code();
}
