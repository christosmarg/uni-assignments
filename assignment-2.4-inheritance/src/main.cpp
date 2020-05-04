#include "appsystem.h"

std::ostream& operator<< (std::ostream& stream, const AppSystem& sys)
{
	std::vector<App *> apps = sys.get_apps();
	for (auto& app : apps)
	{
		Manufacturer m = app->get_manf();
		stream <<
			app->get_serialnum() << " " <<
			app->get_name() << " " <<
			app->get_os() << " " <<
			app->get_price() << " ";

		if (Office *o = dynamic_cast<Office *>(app))
		{
			std::vector<std::string> exts = o->get_exts();
			for (auto& ext : exts)
				stream << ext << " ";		
		}
		else if (Game *o = dynamic_cast<Game *>(app))
		{
			stream <<
				o->get_genre() << " " <<
				(o->get_online() ? "Yes" : "No") << " ";
		}
		
		std::vector<Review *> revs = app->get_revs();
		for (auto& rev : revs)
			stream <<
				rev->get_stars() << " " <<
				rev->get_username() << " " <<
				rev->get_comment() <<  " ";

		stream <<
			m.get_serialnum() << " " <<
			m.get_name() << " " <<
			m.get_email();

		stream << std::endl;
	}
	return stream;	
}

int main(int argc, char **argv)
{
	AppSystem sys;
	Manufacturer *comp = new Manufacturer("124", "Company", "comp@comp.com");
	Manufacturer *chris = new Manufacturer("568", "Chris", "chris@cm.com");
	sys += comp;
	sys += chris;
	if (!sys.read_data<Manufacturer>("res/manfdata.csv")) return -1;
	if (!sys.read_data<App>("res/appdata.csv")) return -1;
	std::vector<std::string> ext = {".doc", ".xls", ".ppt"};
	sys += new Office("459", "OpenOffice", "Linux 2.2", comp, 0, ext);
	sys += new Game("731", "minecurses", "Linux 4.5", chris, 0, "Puzzle", false);

	Review rev(4, "Name Surnaming", "Good");
	sys.newrev("minecurses", &rev);
	std::cout << sys << std::endl;

	sys.removebad(chris);
	std::cout << sys << std::endl;

	std::vector<Office *> fapps = sys.get_freeapps();
	std::vector<Game *> ggames = sys.get_goodgames();
	for (auto& fapp : fapps)
		std::cout << fapp->get_name() << std::endl;
	for (auto& ggame : ggames)
		std::cout << ggame->get_name() << std::endl;

	if (!sys.export_data<Manufacturer>("res/manfdata_out.csv")) return -1;
	if (!sys.export_data<App>("res/appdata_out.csv")) return -1;

	return 0;
}
