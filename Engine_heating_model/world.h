#ifndef _WORLD_H_
#define _WORLD_H_

// В этом классе-синглтоне находится "глобальный" параметр "Температура окружающей среды".
class world
{
public:
	static world& instance()
	{
		static world the_single_instance;
		return the_single_instance;
	}
	double get_env_T() {
		return T_environment;
	}
	void set_env_T(double t) {
		T_environment = t;
	}
private:
	double T_environment;
	world() :T_environment(0) {}
	world(const world&) = delete;
	world& operator=(const world&) = delete;
};

#endif