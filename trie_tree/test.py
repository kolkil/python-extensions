import trieTree

t = trieTree.trieTree()
print(t.insert("test"))
print(t.insert("test"))
print(t.insert("test"))
print(t.insert("test"))
print(t.insert("kij"))
print(t.insert("testowy"))
print(t.insert("kaskader"))

t1 = trieTree.trieTree()
print(t1.insert("test"))
print(t1.insert("test"))

t3 = trieTree.trieTree()
print(t3.insertWithValue("testowa", 3))
print(t3.insert("testowa"))
print(t3.inTree("testowa"))
print(t3.inTree("testowa"))
print(t3.inTree("testowa"))

t.printTree()
t1.printTree()
t3.printTree()

print(t.treeToList())
# print(t1.treeToList())
# print(t3.treeToList())