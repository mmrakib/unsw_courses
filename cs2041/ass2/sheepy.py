#!/usr/bin/python3 -u

###
# COMP2041 Assignment 2
#
# Mohammad Mayaz Rakib (z5361151)
###

import json, re, sys, os

# Check if glob character exists in string
def check_glob_exists(text):
    if ('*' in text or '?' in text or '[' in text or ']' in text):
        return True
    else:
        return False

# Handle string with glob character, if string is to_be_printed, also adds ' '.join() code
def handle_glob(text, to_be_printed=False):
    words = text.split()

    for word in words:
        if (check_glob_exists(word)):
            if ('*' in word):
                text = text.replace(word, f'sorted(glob.glob("{word}"))')
            elif ('?' in text):
                text = text.replace(word, f'sorted(glob.glob("{word}"))')
            elif ('[' in text):
                text = text.replace(word, f'sorted(glob.glob("{word}"))')
            elif (']' in text):
                text = text.replace(word, f'sorted(glob.glob("{word}"))')

    if (to_be_printed):
        for i, word in enumerate(words):
            if (check_glob_exists(word)):
                before_string = ' '.join(words[:i-1])
                after_string = ' '.join(words[i+1:])
                glob_string = words[i]
                output = '" ".join({})'.format(glob_string)
                if (len(before_string) > 0):
                    output = f'"{before_string}" + ' + output
                if (len(after_string) > 0):
                    output += f' + "{after_string}"'
        output = f'print({output})\n'
        return output
    else:
        return text

# Check string for type of quote (none, single, double) that it is enclosed with
def check_quote_type(text):
    if (text.startswith('\"')):
        return 2
    elif (text.startswith('\'')):
        return 1
    else:
        return 0

# Check string to see if it contains any variable
def check_var(text):
    if (re.search('\$', text) or re.search('{}', text)):
        return True
    else:
        return False

# Check string to see if it contains a special variable
def check_special_var(text):
    if (re.search('\$[0-9]+', text) or re.search('\$#', text) or re.search('\$@', text)):
        return True
    else:
        return False

# Handle string containing special variable
def handle_variable(text, enclose=True, in_string=True):
    words = text.strip().split()
    quote_type = check_quote_type(text)
    for i, word in enumerate(words):
        if '$' in word and quote_type != 1:
            idx = word.index('$')
            if word[idx+1].isdigit():
                index = int(word[idx+1:])
                words[i] = '{' + f'sys.argv[{index}]' + '}'
                continue
            elif word[idx+1:] == '#':
                words[i] = '{' + f'len(sys.argv)' + '}'
                continue
            elif word[idx+1:] == '@':
                args = ', '.join(sys.argv[1:])
                words[i] = '{' + f'{args}' + '}'
                continue
            else:
                if (in_string):
                    varname = ''
                    if (word.startswith('${')):
                        m = re.search('{' + '(.+?)' + '}', word)
                        if m:
                            varname = m.group(1)
                            words[i] = re.sub('${.*}', '{' + varname + '}', words[i])
                    else:
                        varname = word[idx+1:]
                        words[i] = '{' + varname + '}'
                else:
                    varname = word[1:-1]
                    words[i] = varname
                continue

    if (enclose):
        return 'f"' + (' '.join(words)) + '"'
    else:
        return ' '.join(words)
    
def check_command_sub(text):
    if (re.search('`', text) or re.search('\$\(', text)):
        return True
    else:
        return False

def handle_command_sub(text):
    words = text.strip().split()
    quote_type = check_quote_type(text)
    for i, word in enumerate(words):
        result = ''
        if (quote_type != 1):
            if word.startswith('`'):
                result = re.search('`(.*)`', text)
            elif word.startswith('$('):
                result = re.search('\$\((.*)\)', text)
            command_string = ''
            if result:
                res_array = result.group(1).split()
                if len(res_array) > 0:
                    for i, res in enumerate(res_array):
                        if (i == len(res_array) - 1):
                            command_string += f'"{res}"'
                        else:
                            command_string += f'"{res}", '
                    if (command_string[0] == '['):
                        command_string += ']'
                if word.startswith('`'):
                    output = re.sub('`.*`', f'subprocess.run({command_string})', text)
                    return output
                elif word.startswith('$('):
                    output = re.sub('\$\(.*\)', f'subprocess.run({command_string})', text)
                    return output
            else:
                return ''

def check_test_condition(condition):
    if (condition.startswith('test')):
        return True
    else:
        return False

def handle_test_condition(condition):
    cond_array = condition.split()
    for i, cond in enumerate(cond_array):
        if (cond.startswith('-')):
            if (cond == '-a'):
                # Check if file exists (legacy)
                output = f'os.path.exists("{cond_array[i + 1]}")'
                return output
            elif (cond == '-d'):
                # Check if directory exists
                output = f'os.path.isdir("{cond_array[i + 1]}")'
                return output
            elif (cond == '-e'):
                # Check if file exists
                output = f'os.path.exists("{cond_array[i + 1]}")'
                return output
            elif (cond == '-f'):
                # Check if file exists and is a regular file
                output = f'os.path.isfile("{cond_array[i + 1]}")'
                return output
            elif (cond == '-h'):
                # Check if file is symbolic link (legacy)
                output = f'os.path.islink("{cond_array[i + 1]}")'
                return output
            elif (cond == '-L'):
                # Check if file is symbolic link
                output = f'os.path.islink("{cond_array[i + 1]}")'
                return output
            elif (cond == '-k'):
                # Check if file is sticky
                output = f'os.stat("{cond_array[i + 1]}").st_mode & 01000 == 01000'
                return output
            elif (cond == '-r'):
                # Check if file is readable
                output = f'os.access("{cond_array[i + 1]}", os.R_OK)'
                return output
            elif (cond == '-s'):
                # Check if file is not empty
                output = f'os.stat("{cond_array[i + 1]}").st_size != 0'
                return output
            elif (cond == '-w'):
                # Check if file is writable
                output = f'os.access("{cond_array[i + 1]}", os.W_OK)'
                return output
            elif (cond == '-x'):
                # Check if file is executable
                output = f'os.access("{cond_array[i + 1]}", os.X_OK)'
                return output
            elif (cond == '-n'):
                # Check if string is not empty
                output = f'len("{cond_array[i + 1]}") != 0'
                return output
            elif (cond == '-z'):
                # Check if string is not empty
                output = f'len("{cond_array[i + 1]}") != 0'
                return output
            elif (cond == '-eq'):
                # Check if numbers are equal
                output = f'{cond_array[i - 1]} == {cond_array[i + 1]}'
                return output
            elif (cond == '-ne'):
                # Check if numbers are not equal
                output = f'{cond_array[i - 1]} != {cond_array[i + 1]}'
                return output
            elif (cond == '-lt'):
                # Check if numbers are less than
                output = f'{cond_array[i - 1]} < {cond_array[i + 1]}'
                return output
            elif (cond == '-le'):
                # Check if numbers are less than or equal to
                output = f'{cond_array[i - 1]} <= {cond_array[i + 1]}'
                return output
            elif (cond == '-gt'):
                # Check if numbers are greater than
                output = f'{cond_array[i - 1]} > {cond_array[i + 1]}'
                return output
            elif (cond == '-ge'):
                # Check if numbers are greater than or equal to
                output = f'{cond_array[i - 1]} >= {cond_array[i + 1]}'
                return output
        elif (cond == '='):
            # Check if strings are equal
            output = f'"{cond_array[i - 1]}" == "{cond_array[i + 1]}"'
            return output
        elif (cond == '!='):
            # Check if strings are not equal
            output = f'"{cond_array[i - 1]}" != "{cond_array[i + 1]}"'
            return output
        elif (cond == '<'):
            # Check if strings are alphabetically less than
            output = f'"{cond_array[i - 1]}" < "{cond_array[i + 1]}"'
            return output
        elif (cond == '>'):
            # Check if strings are alphabetically greater than
            output = f'"{cond_array[i - 1]}" > "{cond_array[i + 1]}"'
            return output
    return condition

class Transpiler(object):
    def __init__(self, filename):
        self.filename = filename
        self.f = open(filename)

        self.input_index = 0
        self.input_lines = self.f.readlines()
        self.input_size = len(self.input_lines)

        self.output_lines = []
        
        self.imports = []
        self.variables = []

        self.ast = []
        self.target_ast = self.ast
        self.saved_node = None

    def __del__(self):
        self.f.close()

    # Creates a string metadata object
    def create_string(self, value):
        string = {
            'type': 'string',
            'value': value,
            'quote_type': check_quote_type(value),
            'variable_refs': [],
            'contains_glob': check_glob_exists(value)
        }
        words = value.split()
        for i, word in enumerate(words):
            if (word.startswith('$')):
                string['variable_refs'].append(word[1:])
        string['value'] = ' '.join(words)
        return string

    # Creates a variable metadata object
    def create_variable(self, name, value):
        variable = {
            'type': 'variable',
            'name': name,
            'value': value
        }
        self.variables.append(variable)
        return variable
    
    # Creates an import metadata object
    def create_import(self, name):
        importobj = {
            'type': 'import',
            'name': name
        }
        if ({'type': 'import', 'name': name} not in self.imports):
            self.imports.append(importobj)
        return importobj

    # Finds first variable metadata object with 'name' as its name and returns it
    def get_variable(self, name):
        for var in self.variables:
            if (var['name'] == name):
                return var
            
    # Finds first import metadata object with 'name' as its library and returns it
    def get_import(self, name):
        for imp in self.imports:
            if (imp['name'] == name):
                return imp

    def check_end_of_input(self):
        if (self.input_index == self.input_size):
            return False
        else:
            return True

    def get_next_line(self):
        line = self.input_lines[self.input_index]
        index = self.input_index
        self.input_index += 1
        return (index, line)

    def parse(self):
        while (self.check_end_of_input()):
            index, line = self.get_next_line()
            elements = line.strip().split()

            # Blank line
            if (len(elements) == 0):
                node = {
                    'type': 'blank'
                }
                self.target_ast.append(node)
                continue
            # Shebang
            elif (elements[0] == '#!/bin/dash'):
                node = {
                    'type': 'shebang'
                }
                self.target_ast.append(node)
                continue
            # Comment
            elif (elements[0].startswith('#')):
                node = {
                    'type': 'comment',
                    'text': line[1:]
                }
                self.target_ast.append(node)
                continue
            # Keywords )
            elif (elements[0] in ['do', 'done', 'then']):
                continue
            # Assignment (creates variable)
            elif ('=' in elements[0]):
                lval = elements[0].split('=')[0]
                rval = elements[0].split('=')[1]
                self.create_variable(lval, self.create_string(rval))

                node = {
                    'type': 'assignment',
                    'lval': lval,
                    'rval': rval
                }
                self.target_ast.append(node)
                continue
            # Echo
            elif (elements[0].startswith('echo')):
                text = ' '.join(elements[1:])
                string = self.create_string(text)
                
                node = {
                    'type': 'echo',
                    'text': string,
                    'is_newline_ended': True
                }
                self.target_ast.append(node)
                continue
            # Exit
            elif (elements[0].startswith('exit')):
                node = {
                    'type': 'exit',
                    'return_val': str(elements[1])
                }
                self.target_ast.append(node)
                continue
            # Change directory
            elif (elements[0].startswith('cd')):
                node = {
                    'type': 'cd',
                    'directory': str(elements[1])
                }
                self.target_ast.append(node)
                continue
            # Read
            elif (elements[0].startswith('read')):
                node = {
                    'type': 'read',
                    'variable_name': elements[1] 
                }
                self.create_variable(node['variable_name'], '')
                self.target_ast.append(node)
                continue
            # For loop
            elif (elements[0] == 'for'):
                iterator = elements[1]
                iterable = ' '.join(elements[3:])

                node = {
                    'type': 'for',
                    'iterator': iterator,
                    'iterable': iterable,
                    'body': []
                }
                
                self.target_ast.append(node)

                self.saved_node = node
                self.target_ast = node['body']
                continue
            # While loop
            elif (elements[0] == 'while'):
                if (elements[1] == 'test'):
                    condition = ' '.join(elements[1:])
                node = {
                    'type': 'while',
                    'condition': condition,
                    'body': []
                }

                self.target_ast.append(node)

                self.saved_node = node
                self.target_ast = node['body']
                continue
            elif (elements[0] == 'done'):
                self.target_ast = self.ast
                self.saved_node = None
                continue
            # If block
            elif (elements[0] == 'if'):
                condition = ' '.join(elements[1:])
                node = {
                    'type': 'if',
                    'condition': condition,
                    'condition2': '',
                    'body_1': [],
                    'body_2': [],
                    'body_3': []
                }
                self.saved_node = node
                self.target_ast = node['body_1']
                continue
            elif (elements[0] == 'elif'):
                self.target_ast = self.saved_node['body_3']
                condition2 = ' '.join(elements[1:])
                self.saved_node['condition2'] = condition2
                continue
            elif (elements[0] == 'else'):
                self.target_ast = self.saved_node['body_2']
                continue
            elif (elements[0] == 'fi'):
                self.target_ast = self.ast
                self.target_ast.append(self.saved_node)
                self.saved_node = None
                continue
            # External commands
            else:
                self.create_import('subprocess')
                arguments = elements[1:]
                if (len(arguments) > 0):
                    for i, arg in enumerate(arguments):
                        if (arg.startswith('$')):
                            arguments[i] = arg[1:]
                
                node = {
                    'type': 'command',
                    'name': elements[0],
                    'arguments': arguments,
                }
                self.target_ast.append(node)
                continue

    def run_parse(self):
        self.parse()

        if (self.ast[1]['type'] != 'blank'):
            self.ast.insert(1, {'type': 'blank'})

    def generate(self, ast, spaces=0):
        if (ast == None):
            return

        include_blanks = False

        indent = ''
        for i in range(0, spaces):
            indent += ' '

        for nodenumber, node in enumerate(ast):
            # Shebang
            if (node['type'] == 'shebang'):
                include_blanks = True
                output = '#!/usr/bin/python3 -u\n'
                self.output_lines.append(output)
                continue
            # Blank line
            elif (node['type'] == 'blank'):
                if (include_blanks):
                    output = '\n'
                    output = indent + output
                    self.output_lines.append(output)
                    include_blanks = False
                continue
            # Assignment
            elif (node['type'] == 'assignment'):
                include_blanks = True
                output = '{} = "{}"\n'.format(node['lval'], node['rval'])
                output = indent + output
                self.output_lines.append(output)
                continue
            # Echo
            elif (node['type'] == 'echo'):
                include_blanks = True
                string = node['text']
                if (string['contains_glob'] == True and string['quote_type'] != 1):
                    self.create_import('glob')
                    output = handle_glob(string, to_be_printed=True)
                    output = f'print({output})\n'
                    output = indent + output
                    self.output_lines.append(output)
                elif (check_var(string['value'])):
                    if (check_special_var(string['value'])):
                        self.create_import('sys')
                    var_string = handle_variable(string['value'], enclose=True)
                    output = 'print({})\n'.format(var_string)
                    output = indent + output 
                    self.output_lines.append(output)
                else:
                    output = 'print("{}")\n'.format(string['value'])
                    output = indent + output
                    self.output_lines.append(output) 
                continue
            # Exit
            elif (node['type'] == 'exit'):
                include_blanks = True
                self.create_import('sys')
                output = f'sys.exit({node["return_val"]})\n'
                output = indent + output
                self.output_lines.append(output)
            # Change directory
            elif (node['type'] == 'cd'):
                include_blanks = True
                self.create_import('os')
                output = f'os.chdir("{node["directory"]}")\n'
                output = indent + output
                self.output_lines.append(output)
            # Read
            elif (node['type'] == 'read'):
                include_blanks = True
                output = f'{node["variable_name"]} = input()\n'
                output = indent + output
                self.output_lines.append(output)
            # Command
            elif (node['type'] == 'command'):
                include_blanks = True
                command_string = '['
                arguments = node['arguments']
                arguments.insert(0, node['name'])
                if len(arguments) > 0:
                    for i, arg in enumerate(arguments):
                        if (i == len(node['arguments']) - 1):
                            command_string += f'"{arg}"'
                        else:
                            command_string += f'"{arg}", '
                command_string += ']'
                output = f'subprocess.run({command_string})\n'
                output = indent + output
                self.output_lines.append(output)
            # For loop
            elif (node['type'] == 'for'):
                include_blanks = True
                iterable_string = ''
                if (check_glob_exists(node['iterable']) and check_quote_type(node['iterable']) != 1):
                    self.create_import('glob')
                    iterable_string = handle_glob(node['iterable'])
                    if ('os' in output):
                        self.create_import('os')
                else:
                    iterable_array = node['iterable'].split()
                    iterable_string = '['
                    for i, iterable_element in enumerate(iterable_array):
                        if (i == len(iterable_array) - 1):
                            iterable_string += f'"{iterable_element}"'
                        else:
                            iterable_string += f'"{iterable_element}", '
                    iterable_string += ']'

                output = f'for {node["iterator"]} in {iterable_string}:\n'
                output = indent + output
                self.output_lines.append(output)
                self.generate(node['body'], spaces + 4)
                continue
            # While loop
            elif (node['type'] == 'while'):
                include_blanks = True
                output = ''
                if (check_test_condition(node['condition'])):
                    output = handle_test_condition(node['condition'])
                    if ('os' in output):
                        self.create_import('os')
                output = handle_variable(output, enclose=False, in_string=False)
                output = f'while {output}:\n'
                output = indent + output
                self.output_lines.append(output)
                self.generate(node['body'], spaces + 4)
            # If block
            elif (node['type'] == 'if'):
                include_blanks = True
                output = ''
                if (check_test_condition(node['condition'])):
                    self.create_import('os')
                    output = handle_test_condition(node['condition'])
                output = handle_variable(output, enclose=False, in_string=False)
                output = f'if {output}:\n'
                output = indent + output
                self.output_lines.append(output)
                
                self.generate(node['body_1'], spaces + 4)

                if (node['condition2'] != ''):
                    output = ''
                    if (check_test_condition(node['condition2'])):
                        self.create_import('os')
                        output = handle_test_condition(node['condition2'])
                    output = handle_variable(output, enclose=False, in_string=False)
                    output = f'elif {output}:\n'
                    output = indent + output
                    self.output_lines.append(output)
                    self.generate(node['body_3'], spaces + 4)

                output = f'else:\n'
                output = indent + output
                self.output_lines.append(output)


                self.generate(node['body_2'], spaces + 4)

                output = f'fi\n'
                output = indent + output
                self.output_lines.append(output)
            else:
                pass
    
    def run_generate(self):
        self.generate(self.ast)

        # Add import statement once all code has been read and we know what to import
        if (len(self.imports) > 0):
            import_string = f'\nimport '
            for i, imp in enumerate(self.imports):
                if (i == len(self.imports) - 1):
                    import_string += f'{imp["name"]}'
                else:
                    import_string += f'{imp["name"]}, '
            import_string += '\n'
            self.output_lines.insert(1, import_string)

    def print(self):
        for line in self.output_lines:
            print(line, end='')

    def debug_print(self, print_input=True, print_ir=True, print_output=True, print_extra=True):
        if (print_input):
            print('Input file:')
            for line in self.input_lines:
                print(line, end='')
            print('')
        if (print_extra):
            print('Imports:')
            print(json.dumps(self.imports, indent=4))
            print('')
        if (print_extra):
            print('Variables')
            print(json.dumps(self.variables, indent=4))
            print('')
        if (print_ir):
            print('IR:')
            print(json.dumps(self.ast, indent=4))
            print('')
        if (print_output):
            print('Output:')
            for line in self.output_lines:
                print(line, end='')

if __name__ == '__main__':
    if (len(sys.argv) != 2):
        print('Usage: ./sheepy.py <filename>')
        exit(1)
    
    filename = sys.argv[1]
    if not os.path.exists(filename):
        print('Filename does not exist')
        exit(1)

    t = Transpiler(filename)
    t.run_parse()
    t.run_generate()
    t.print()
