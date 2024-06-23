def parse_input(file_name):
    return_file = list()
    try:
        with open(file_name, 'r') as file:
            for line in file:
                row = line.strip().split(",")

                individual = dict()

                name_surname = row[0].split(" ")
                name = name_surname[0]
                name_surname.remove(name)

                individual["name"] = name
                individual["surname"] = " ".join(name_surname)
                individual["country"] = row[1]
                individual["age"] = int(row[2])
                individual["date_of_birth"] = row[3]
                individual["nickname"] = row[4]
                individual["ranking"] = int(row[5])

                return_file.append(individual)

    except FileNotFoundError:
        print("DAT_GRESKA", end="")
        return "DAT_GRESKA"

    return return_file


def print_out(arr):
    for person in arr:
        print("{:3}: {} {} {}".format(person["ranking"], person["name"], person["nickname"].upper(), person["surname"]))


def country(arr):
    countries = list()
    countries_list = list()
    for person in arr:
        if person["country"] in countries_list:  # ['country name', number of players, ranking, average]
            i = countries_list.index(person["country"])
            countries[i][1] += 1  # update number of players
            countries[i][2] += person["ranking"]  # update ranking for a country

        else:
            countries.append([person["country"], 1, person["ranking"]])
            countries_list.append(person["country"])

    for i in range(0, len(countries)):
        countries[i].append(float(countries[i][2] / countries[i][1]))

    return countries


def write_file(cdata, file_name):
    with open(file_name, "w") as file:
        for item in cdata:  # ['country name', number of players, ranking, average]
            line = "{} {} {:.2f}".format(item[0], item[1], item[3])
            file.write(line + "\n")


# //////////////////////////////////////
# main program start executing from here
# //////////////////////////////////////

# standard input
input_file = input() + ".csv"
output_file = input() + ".txt"

data = parse_input(input_file)

if data != "DAT_GRESKA":
    data.sort(key=lambda x: x["ranking"])

    print_out(data)

    country_data = country(data)

    country_data.sort(key=lambda x: (x[1], x[2]), reverse=True)

    write_file(country_data, output_file)
