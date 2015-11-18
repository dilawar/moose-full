import moogli
import unittest

# network1 = moogli.Network("Network-1")
# network2 = moogli.Network("Network-2")

# print(network1)
# print(network2)

# assert network1 != network2

# for i in range(0, 10):
#     neuron = moogli.Neuron("Neuron-" + str(i))
#     network1.attach(neuron)
#     print(neuron)

# for i in range(10, 20):
#     neuron = moogli.Neuron("Neuron-" + str(i))
#     network2.attach(neuron)
#     print(neuron)

# assert(network1.size() == 10)
# assert(network2.size() == 10)

class NetworkTests(unittest.TestCase):
    def setUp(self):
        self.networks = []
        self.network  = moogli.Network("network")
        for i in range(10):
            self.networks.append(moogli.Network(str(i)))

    def tearDown(self):
        self.networks = None

    def test_id(self):
        for i in range(10):
            self.assertTrue(self.networks[i].id, str(i))

    def test_visibility(self):
        self.network.show()
        self.assertTrue(self.network.visible())
        self.network.hide()
        self.assertFalse(self.network.visible())

    def test_length(self):
        self.assertTrue(len(self.network), 0)
        self.network.attach(moogli.Neuron("a"))
        self.assertTrue(len(self.network), 1)
        self.network.attach(moogli.Neuron("b"))
        self.assertTrue(len(self.network), 2)

def main():
    unittest.main()

if __name__ == "__main__":
    main()
