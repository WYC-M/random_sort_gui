from random import shuffle


class random_sort:
    input_path: str = None
    output_path: str = None
    origin_data: str = None
    finally_list: list = None

    def __init__(self, path_in, path_out):
        self.input_path = path_in
        self.output_path = path_out

    def read_list(self):
        with open(self.input_path, "r", encoding="UTF8") as f:
            self.origin_data = f.readline().replace("\n", "").replace("\r", "")

    def shuffle_list(self):
        self.finally_list = self.origin_data.split(",")
        shuffle(self.finally_list)

    def write_list(self):
        with open(self.output_path, "w", encoding="UTF-8") as f:
            i = 0
            while i < len(self.finally_list) - 1:
                f.write(self.finally_list[i])
                f.write(", ")
                i += 1
            f.write(self.finally_list[i].strip())
            f.write("\n")


if __name__ == "__main__":
    rs = random_sort("list.txt", "data.txt")
    rs.read_list()
    rs.shuffle_list()
    rs.write_list()
