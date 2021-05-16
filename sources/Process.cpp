// Copyright 2021 Danil Postvaykin <postvaykin01@mail.ru>
#include <Process.hpp>

Process::Process() {}

void Process::out_help()
{
  std::string out_info;
  out_info = R"(Usage: builder [options]
Allowed options:
  --help                    : выводим вспомогательное сообщение
  --config <Release|Debug>  : указываем конфигурацию сборки (по умолчанию Debug)
  --install                 : добавляем этап установки (в директорию _install)
  --pack                    : добавляем этап упаковки (в архив формата tar.gz)
  --timeout <count>         : указываем время ожидания (в секундах))";

  std::cout << out_info << std::endl;
}

void Process::start_process(bool _install, bool _pack, std::string _config,
                            int _time)
{
  std::thread{std::bind(&Process::timer, this, _time)}.detach();
  bool success = my_task(
      "-H. -B_builds -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=" +
      _config);
  if (!success)
    return;
  success = my_task("--build _builds");
  if (!success)
    return;
  if (_install&&_pack)
  {
    success = my_task("--build _builds --target install");
    if (!success)
      return;
    success = my_task("--build _builds --target package");
    if (!success)
      return;
  } else if (_install) {
    success = my_task("--build _builds --target install");
    if (!success)
      return;
  } else if (_pack) {
    success = my_task("--build _builds --target package");
    if (!success)
      return;
  }
}

bool Process::my_task(std::string task)
{
  auto cmake_path = boost::process::search_path("cmake");

  boost::process::ipstream stream;
  boost::process::child child(cmake_path.string() + " " + task,
                  boost::process::std_out > stream);
  new_child = std::move(child);

  for (std::string line; new_child.running() &&
                         std::getline(stream, line);)
  {
    std::cout << line << std::endl;
  }

  new_child.wait();

  auto exit_code = new_child.exit_code();

  if (exit_code != 0) {
    std::cout << "Non zero exit code. Exiting..." << std::endl;
    return false;
  } else {
    return true;
  }
}

void Process::timer(int time)
{
  sleep(time);
  new_child.terminate();
}

Process::~Process() {}
