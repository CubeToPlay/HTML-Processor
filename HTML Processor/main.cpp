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

void read_line(ifstream& file, char* char_array, string &container)
{
	file.getline(char_array, MAX_LENGTH);
	container = parse(char_array, "=\"", "\"");
}

void manage_directory(string& file_template, string& container, string& output_location, string& extension)
{
	for (const auto& origin_file_entry : fs::directory_iterator(container))
	{
		if (fs::is_regular_file(origin_file_entry.path()))
		{
			string origin_file_path = origin_file_entry.path().string();
			string origin_file_contents, template_source_file_contents, fill_target, fill_value;
			stringstream buffer;
			vector<string> fill_variables;
			int
				begin_index = origin_file_path.find_last_of("\\") + 1,
				end_index = origin_file_path.find(extension),
				fill_index;

			if (end_index != -1)
			{
				fs::path output_path = fs::path(output_location) / (origin_file_path.substr(begin_index, end_index - begin_index) + ".html");
				std::cout << output_path << std::endl;

				ifstream  template_source_file(file_template, std::ios::binary);
				ofstream  destination_file(output_path, std::ios::binary);
				fstream origin_file(origin_file_path);

				buffer << origin_file.rdbuf();
				origin_file_contents = buffer.str();

				buffer.str("");

				buffer << template_source_file.rdbuf();
				template_source_file_contents = buffer.str();

				while (end_index != -1 && begin_index != -1)
				{
					begin_index = template_source_file_contents.find("@{") + 2;
					end_index = template_source_file_contents.find("}@");

					if (end_index != -1 && begin_index != -1)
					{
						fill_target = "@" + template_source_file_contents.substr(begin_index, end_index - begin_index);

						fill_variables.push_back(fill_target);

						begin_index -= 2;

						template_source_file_contents.replace(begin_index, (end_index - begin_index) + 2, fill_target);
					}
				}

				//cout << fill_variables.size() << endl;

				for (int i{ 0 }; i < fill_variables.size(); i++)
				{
					fill_target = fill_variables[i];

					begin_index = origin_file_contents.find(fill_target + "{") + fill_target.length() + 1;
					end_index = origin_file_contents.find("}" + fill_target);

					fill_index = template_source_file_contents.find(fill_target);
					if (begin_index != -1 && end_index != -1)
					{
						output(fill_target);
						fill_value = origin_file_contents.substr(begin_index, end_index - begin_index);
					}
					else
						fill_value = "";

					if (fill_index != -1)
						template_source_file_contents.replace(fill_index, fill_target.length(), fill_value);

				}

				//cout << template_source_file_contents << endl;

				destination_file << template_source_file_contents;

				template_source_file.close();
				destination_file.close();
				origin_file.close();
			}
		}
	}
}

int main() {
	output("--Program Started--");

	bool valid_config{ true };

	fs::path config_file_path = std::filesystem::current_path() / "config.cfg";

	string 
		template_file_path,
		pages_file_path,
		output_file_path,
		target_extension;

	char file_line[MAX_LENGTH];

	ifstream config_file(config_file_path);

	if (config_file.is_open())
	{
		output("config file is open!");
		output("reading config file!");

		read_line(config_file, file_line, template_file_path);
		read_line(config_file, file_line, pages_file_path);
		read_line(config_file, file_line, output_file_path);
		read_line(config_file, file_line, target_extension);

		if (fs::is_regular_file(template_file_path))
		{ output("template found!"); }
		else
		{ output("template not found!"); valid_config = false;}

		if (fs::is_directory(output_file_path))
		{ output("output location found!"); }
		else
		{ output("output location not found!"); valid_config = false; }

		if (fs::is_directory(pages_file_path))
		{ output("page directory found!"); }
		else
		{ output(pages_file_path + " is not a valid directory!"); valid_config = false; }

		output("The target extension is " + target_extension);

		if (valid_config)
		{ manage_directory(template_file_path, pages_file_path, output_file_path, target_extension); }
		
	}
	else
	{
		ofstream new_config_file(config_file_path);
		output("config file failed to open!");
		output("creating a config file!");
		new_config_file << "template=\npages=\noutput=\ntarget-extension=\".txt\"";
		output("config file created!");
	}

	config_file.close();


	output("--Done!--");

	system("pause");
	return 0;
}