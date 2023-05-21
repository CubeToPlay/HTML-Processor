#include "main.h"

void output(string output_value)
{
	cout << output_value << endl;
}

string parse(string input, string first_string, string last_string)
{
	int first = input.find(first_string) + first_string.length();
	int last = input.find_last_of(last_string);

	string new_string = input.substr(first, last - first);

	return new_string;
}

void read_line(fstream& file, char* char_array, string &container)
{
	file.getline(char_array, MAX_LENGTH);
	container = parse(char_array, "=\"", "\"");
}

void manage_directory(string& file_template, string& container, string& output_location)
{
	output("reading --- " + container);

	if (fs::is_directory(container) && fs::is_directory(output_location))
	{

		for (const auto& origin_file_entry : fs::directory_iterator(container))
		{
			if (fs::is_regular_file(origin_file_entry.path()))
			{
				string origin_file_path = origin_file_entry.path().string();

				int first = origin_file_path.find_last_of("\\") + 1;
				int last = origin_file_path.find_last_of(".txt") - 3;

				fs::path output_path = fs::path(output_location) / (origin_file_path.substr(first, last - first) + ".html");
				std::cout << output_path << std::endl;

				ifstream  template_source_file(file_template, std::ios::binary);
				ofstream  destination_file(output_path, std::ios::binary);

				fstream origin_file(origin_file_path);
				
				stringstream buffer;
				buffer << origin_file.rdbuf();
				string origin_file_contents = buffer.str();

				buffer.str("");
				buffer << template_source_file.rdbuf();

				string template_source_file_contents = buffer.str();
				int index = template_source_file_contents.find("@body");

				if (index != -1)
				{
					template_source_file_contents.replace(index, 5, origin_file_contents);
				}

				destination_file << template_source_file_contents;

				template_source_file.close();
				destination_file.close();
				origin_file.close();
			}
		}
	}
	else
	{
		output(container + " is not a directory");
	}
}

int main() {
	output("Program Started");

	fs::path config_file_path = std::filesystem::current_path() / "config.cfg";

	string 
		template_file_path,
		pages_file_path,
		output_file_path;

	char file_line[MAX_LENGTH];

	fstream config_file(config_file_path);

	if (config_file.is_open())
	{
		output("config file is open");
		output("reading file");

		read_line(config_file, file_line, template_file_path);
		read_line(config_file, file_line, pages_file_path);
		read_line(config_file, file_line, output_file_path);

		output("file was read");

		if (fs::is_regular_file(template_file_path))
		{
			output("template found!");
		}

		manage_directory(template_file_path, pages_file_path, output_file_path);

	}
	else
	{
		output("config file failed to open");
		output("creating a config file");
		config_file << "template=\npages=\noutput=";
		output("config file created");
	}

	config_file.close();


	output("Done");

	return 0;
}