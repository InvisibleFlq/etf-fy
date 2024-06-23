# helper functions
def load_data(j, arr):
    for i in range(0, j):
        current = input().split(",")

        if len(current) != 3:
            break

        current[1] = int(current[1])    # income
        current[2] = int(current[2])    # payout

        arr.append(current)

    return arr


def calc_share(j, arr):
    for i in range(0, j):
        arr[i].append(arr[i][2] / arr[i][1])

    return arr


def print_out(j, arr, perc):
    for i in range(0, j):

        if arr[i][3] >= perc:
            print("{} - {:.2f}".format(arr[i][0], arr[i][3]))

        else:
            continue


# //////////////////////////////////////
# main program start executing from here
# //////////////////////////////////////

# number of item in the list
k = int(input())

if k > 0:

    data = []

    load_data(k, data)

    if len(data) == k:

        calc_share(k, data)

        data.sort(key=lambda x: x[3], reverse=True)

        percentage = float(input())

        if int(percentage * 100) > 0 and int(percentage * 100) < 100:

            print_out(k, data, percentage)
