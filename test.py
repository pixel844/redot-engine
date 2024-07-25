import json
import os

with open('compile_commands.json', 'r') as f:
    data = json.load(f)

directories = set()
for item in data:
    directories.add(os.path.dirname(item['file']))

with open('gdb_directories.gdb', 'w') as f:
    for directory in directories:
        f.write('directory ' + directory + '\n')