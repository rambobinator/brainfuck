import sys

MEMORY_SIZE = 666


def get_bracket_positions(s, tokens=('[', ']')):
    stack = []
    ret = {}
    start, end = tokens
    for i, c in enumerate(s):
        if c == start:
            stack.append(i)
        elif c == end:
            i_s = stack.pop()
            ret[i_s] = i
            ret[i] = i_s
    return ret


class Interpreter:

    def __init__(self, mem_size=MEMORY_SIZE):
        self.code = None
        self.code_ptr = 0

        self.mem_size = mem_size
        self.mem = [0] * mem_size
        self.ptr = 0

        self.instructions_dict = {
            '>': self.increment_ptr,
            '<': self.decrement_ptr,
            '+': self.increment_val,
            '-': self.decrement_val,
            '.': self.print_val,
            ',': self.read_val,
            '[': self.start_loop,
            ']': self.end_loop
        }

    def run(self, code=None):
        if code:
            self.code = code
        self.code_braces_pos = get_bracket_positions(self.code)
        while self.code_ptr < len(self.code):
            c = self.code[self.code_ptr]
            if c in self.instructions_dict:
                self.instructions_dict[c]()
            self.code_ptr += 1

    def increment_ptr(self, step=1):
        self.ptr += step

    def decrement_ptr(self, step=1):
        self.ptr -= step

    def increment_val(self, step=1):
        self.mem[self.ptr] += step

    def decrement_val(self, step=1):
        self.mem[self.ptr] -= step

    def print_val(self):
        sys.stdout.write(chr(self.mem[self.ptr]))

    def read_val(self):
        entry = sys.stdin.readline()
        for c in entry[:-1]:
            self.mem[self.ptr] = ord(c)
            self.increment_ptr()

    def start_loop(self):
        if self.mem[self.ptr] <= 0:
            self.code_ptr = self.code_braces_pos[self.code_ptr]

    def end_loop(self):
        self.code_ptr = self.code_braces_pos[self.code_ptr] - 1

if __name__ == "__main__":
    if len(sys.argv) == 2:
        interpreter = Interpreter()
        interpreter.run(sys.argv[1])
    else:
        print("Usage: interpreter.py \",[,.]\"")
