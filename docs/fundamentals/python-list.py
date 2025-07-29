a = "hel"

print("operator [i]: Single indexing (len=3)")
print("  {-3, ..., 2}: Valid")
print("  {..., -4} U {3, ...}: `IndexError: list index out of range`")
print()

out = "|  i  | a[i]|\n"
out += "|-----|-----|\n"
for i in range(-3, 3):
    out += f"| {i:3d} | {a[i]:4s}|\n"
print(out)
print("\n")

print("operator [i,j]: Double indexing (len=3)")
print("  i: {-3, ..., 2}: Valid" ",  j: {-2, ..., 3}: Valid")
print("  i:    {..., -4}: Repetition" ",  j:    {..., -3}: Empty")
print("  i:     {3, ...}: Empty" ",  j:     {4, ...}: Repetition")
print()

out = "|     | "
for j in range(-4, 5):
    out += f"{j:10d} | "
out += "\n"

out += "|-----|-"
for j in range(-4, 5):
    out += "-----------|-"
out += "\n"

for i in range(-4, 5):
    out += f"| {i:3d} | "
    for j in range(-4, 5):
        cell = f" {a[i:j]} "
        if cell.strip(" ") == "":
            cell = "."
        out += f"{cell:10s} | "
    out += "\n"

print(out)
