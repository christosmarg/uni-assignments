#include "datahandler.h"

DataHandler::DataHandler()
	:misscount(0), errcount(0) {}

DataHandler::~DataHandler()
{
	dealloc<Course>(courses);
	dealloc<Student>(studs);
	eqvs.clear();
}

bool
DataHandler::store_data()
{
	std::ifstream f;
	f.exceptions(std::ifstream::badbit);
	try
	{
		if (!valid_path(datapath))
			throw std::runtime_error(err_csv(datapath).cstr());
		f.open(datapath);
		if (f.is_open())
		{
			std::cout << "Importing data from \'" << datapath << "\'." << std::endl;
			std::cout << std::endl;
			std::cout << "Making data structures and analyzing data." << std::endl;

			lab::xstring skip;
			lab::getline(f, skip);

			lab::xstring AM, code, grade;
			lab::getline(f, AM, ';');
			lab::getline(f, code, ';');
			lab::getline(f, grade);

			std::map<lab::xstring, Student *>::const_iterator its = studs.find(AM);
			std::map<lab::xstring, Course *>::const_iterator itc = courses.find(code);
			if (its != studs.end() && itc != courses.end())
				grds.insert(std::make_pair(courses[code], std::atof(grade.cstr())));

			while (f.good())
			{
				lab::xstring currAM = AM;
				while (currAM == AM)
				{
					lab::getline(f, AM, ';');
					lab::getline(f, code, ';');
					lab::getline(f, grade);
					if (f.eof()) break;
					if (!analyze(currAM, AM, code, std::atof(grade.cstr()))) break;
				}
			}
		}
		f.close();	
	}
	catch (const std::ifstream::failure& e)
	{
		std::cerr << err_read(datapath) << std::endl << e.what() << std::endl;
		return false;
	}
	return true;
}

bool
DataHandler::analyze(
		const lab::xstring& currAM,
		lab::xstring& AM,
		lab::xstring& code,
		float grade)
{
	std::map<lab::xstring, Student *>::const_iterator its = studs.find(AM);
	std::map<lab::xstring, Course *>::const_iterator itc = courses.find(code);
	if (its != studs.end() && itc != courses.end())
	{
		if (currAM != AM)
		{
			data.insert(std::make_pair(studs[currAM], grds));
			grds.clear();
			grds.insert(std::make_pair(courses[code], grade));
			return false;
		}
		grds.insert(std::make_pair(courses[code], grade));
	}
	else if (its == studs.end())
	{
		errlog.write(ErrLog::ErrType::STUDENT_MISSING, AM);
		errcount++;
	}
	else if (itc == courses.end())
	{
		errlog.write(ErrLog::ErrType::COURSE_MISSING, code);
		errcount++;
	}

	if (its != studs.end() && itc != courses.end())
	{
		miss(AM, code, grade);	
		diffr(AM, code, grade);
	}

	return true;
}

void
DataHandler::miss(lab::xstring AM, lab::xstring code, float grade)
{
	if (code.front() == 'P')
	{
		std::map<lab::xstring, lab::xstring>::const_iterator it = eqvs.find(code);
		if (it != eqvs.end())
		{
			bool found = false;
			for (const auto& grd : grds)
				if (grd.first->get_code() == eqvs[code])
					found = true;
			if (!found)
			{
				missing.push_back(AM + ";" +
						studs[AM]->get_lname() + ";" +
						studs[AM]->get_fname() + ";" +
						courses[eqvs[code]]->get_code() + ";" +
						courses[eqvs[code]]->get_name() + ";" +
						eqvs[code] + ";" +
						courses[code]->get_name() + ";" +
						lab::to_xstr<float>("%.1f", grade));
				misscount++;
			}
		}
	}
}

void
DataHandler::diffr(lab::xstring AM, lab::xstring code, float grade)
{
	std::map<Course *, float>::const_iterator it = grds.find(courses[code]);
	if (it != grds.end() && it->second != grade)
	{
		errlog.write(ErrLog::ErrType::DIFFERENT_GRADES,
				lab::xstring(AM + " in " + code + ": " +
					lab::to_xstr<float>("%.1f", it->second) + " | " +
					lab::to_xstr<float>("%.1f", grade)));
		errcount++;
	}
}

bool
DataHandler::make_report() const
{
	std::ofstream f;
	f.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try
	{
		if (!valid_path(reppath))
			throw std::runtime_error(err_csv(reppath).cstr());
		f.open(reppath);
		if (f.is_open())
		{
			std::cout << "Making report." << std::endl;
			f << "AM;Last name;First name;New course code;New course name;" <<
				 "Old course code;Old course name;Grade" << std::endl;
			for (const auto& m : missing) f << m << std::endl;
			f.close();
		}
	}
	catch (const std::ofstream::failure& e)
	{
		std::cerr << err_write(reppath) << std::endl << e.what() << std::endl;
		return false;
	}
	return true;
}


void
DataHandler::summary() const
{
	int datacount = 0;
	for (const auto& dat : data)
		datacount += dat.second.size();

	std::cout << std::endl;
	std::cout << "Students: " << studs.size() << std::endl;
	std::cout << "Courses: " << courses.size() << std::endl;
	std::cout << "Equivalencies: " << eqvs.size() << std::endl;
	std::cout << "Total grades stored: " << datacount << std::endl;
	std::cout << "Grades missing: " << misscount << std::endl;
	std::cout << "Errors: " << errcount << std::endl;
	std::cout << std::endl;
	std::cout << "Thank you :)" << std::endl;
}

bool
DataHandler::valid_path(const char *fpath) const
{
	return (strstr(fpath, ".csv") != nullptr);
}

const lab::xstring 
DataHandler::err_csv(const char *fpath) const
{
	return lab::xstring("Error. File must be of format \'.csv\'. ()").
		insert(fpath, 39);
}

const lab::xstring
DataHandler::err_read(const char *fpath) const
{
	return lab::xstring("Error reading file \'\'.").insert(fpath, 20);
}

const lab::xstring
DataHandler::err_write(const char *fpath) const
{
	return lab::xstring("Error writing to file \'\'.").insert(fpath, 23);
}
