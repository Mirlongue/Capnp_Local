import os
import json
import subprocess

cmd = [
    'capnp',
    'convert',
    'text:canonical',
    'sample.capnp',
    'Sample']

def json_to_txt(data, indent_level = 0):
    def format_value(value, indent_level):
        if isinstance(value, dict):
            return json_to_txt(value, indent_level + 1)
        elif isinstance(value, str):
            return f'"{value}"'
        elif isinstance(value, bool):
            return 'true' if value else 'false'
        elif isinstance(value, list):
            return f'[{", ".join(map(lambda x: format_value(x, indent_level), value))}]'
        else:
            return str(value)
    formatted_pairs = []
    for key, value in data.items():
        indent = '    ' * indent_level
        formatted_pairs.append(f'{indent}{key} = {format_value(value, indent_level)}')
    if indent_level == 0:
        return '(\n' + ',\n'.join(formatted_pairs) + '\n)'
    else:
        return '(\n' + ',\n'.join(formatted_pairs) + '\n' + '    ' * indent_level + ')'

if __name__ == '__main__':

    script_path = os.path.abspath(__file__)
    current_dir = os.path.dirname(script_path)

    json_path = os.path.join(current_dir, 'sample.json')
    with open(json_path, 'r', encoding='utf-8') as file:
        json_data = json.load(file)
    print(json_data)

    txt_data = json_to_txt(json_data)

    txt_path = os.path.join(current_dir, 'sample.txt')
    with open(txt_path, 'w', encoding='utf-8') as file:
        file.write(txt_data)

    dat_path = os.path.join(current_dir, 'sample.dat')
    with open(txt_path, 'r') as input_file:
        with open(dat_path, 'w') as output_file:
            subprocess.run(cmd, stdin = input_file, stdout = output_file, stderr = subprocess.PIPE, shell = True)

    os.remove(txt_path)