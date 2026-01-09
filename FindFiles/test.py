def add_titles_to_lines():
    with open('file_list.txt', 'r') as file:
        lines = file.readlines()

    new_lines = []
    current_title = ""
    for line in lines:
        line = line.strip()
        if line == "":
            current_title = ""
        elif not current_title:
            current_title = line
        else:
            new_lines.append(f"{current_title}: {line}\n")

    with open('file_list.txt', 'w') as file:
        file.writelines(new_lines)

if __name__ == "__main__":
    add_titles_to_lines()