# HTML-Processor

Setup
1. Run the program once to create a config.cfg file.
2. Set the value "template" in config.cfg to the template file.
3. Set the value "pages" in config.cfg to the folder that contains the source files.
4. Set the value "output" in config.cfg to the folder that you want the files to end up.
5. The "target-extension" in config.cfg is the file extension of the source files you want to read from. 

Using The Program
- Set the value you want to replace in the template file inside @{}@
  Ex: @{replacement_value}@
- The source file that is read from should becontained in @value{}@value
  Ex: @replacment_value{ any value }@replacment_value
