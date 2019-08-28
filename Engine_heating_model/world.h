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
	bool set_env_T(double t) {
		T_environment = t;
	}
private:
	double T_environment;
	world() :T_environment(0) {}
	//world(double t) :T_environment(t) {}
	world(const world& root) = delete;
	world& operator=(const world&) = delete;
};