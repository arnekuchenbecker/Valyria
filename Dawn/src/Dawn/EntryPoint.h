#pragma once

#ifdef DW_PLATFORM_WINDOWS

extern Dawn::Application* Dawn::createApplication();

int main(int argc, char** argv)
{
	Dawn::Log::init();
	DW_CORE_INFO("Initialized Core Log");
	DW_CORE_INFO("Initialized Client Log");

	auto app = Dawn::createApplication();
	app->run();
	delete app;
}

#endif