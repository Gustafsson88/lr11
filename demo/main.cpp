#include <Process.hpp>

int main(int argc, char **argv) {
  Process proc;

  std::string config = "";
  bool install = false;
  bool pack = false;
  int timeout;

  options_description desc("General options");
  std::string task_type;

  desc.add_options()
      ("help", "")
      ("config", boost::program_options::value<std::string>()->
          default_value("Debug"))
      ("install", "")
      ("pack", "")
      ("timeout",boost::program_options::value<int>()->
          default_value(100));

  variables_map vm;
  try
  {
    parsed_options parsed = command_line_parser(argc, argv).options(desc).
        allow_unregistered().run();
    store(parsed, vm);

    if (vm.count("help")) {
      proc.out_help();
      return 1;
    }
    if (vm.count("install")) {
      install = true;
    }
    if (vm.count("pack")) {
      pack = true;
    }

    config = vm["config"].as<std::string>();
    timeout = vm["timeout"].as<int>();

    proc.start_process(install, pack, config, timeout);
  }
  catch (std::exception& ex)
  {
    std::cout << desc << std::endl;
  }
}