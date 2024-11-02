import matplotlib.pyplot as plt


speeds1 = [ # step response without friction pi controller
    0, 1243, 1560, 1628, 1749, 1825, 1868, 1904, 1929, 
    1939, 1987, 1975, 1969, 1956, 1988, 1990, 1984, 
    2020, 2004, 2001, 1988, 1995, 1992, 2010
]

speeds2 = [ # step response with friction pi controller
    0, 1266, 1545, 1590, 1765, 1834, 1879, 1893, 1931,
    1955, 1958, 1992, 1984, 2002, 1968, 1992, 1995,
    1990, 1544, 1873, 2216, 2080, 2066, 2056, 2039,
    2018, 2017, 2018, 2000, 1438, 1291, 1196, 1373,
    2627, 2447, 2355, 2301, 2240, 2209, 2162, 2124,
    2098, 2050, 2027, 2007, 2021, 2024, 2026, 2024,
    2008, 2008, 2011, 2029, 2014, 2005, 2011
]

speeds3 = [ # step response proportional controller
    0, 2800, 722, 2181, 544, 2337, 604, 2296, 594,
    2307, 590, 2306, 589, 2313, 591, 2308, 581,
    2313
]

speeds4 = [ # step response with friction proportional controller
    0, 2800, 726, 2174, 579, 2304, 615, 2287, 611,
    2302, 609, 1369, 1606, 766, 2208, 572, 2325,
    611, 1020, 1040, 2094
]



time = list(range(len(speeds1)))


plt.axhline(y=2000, color='r', linestyle='--', label="Reference Speed (2000 pps)")


plt.plot(time, speeds1, marker='o', label='Speed')


plt.title("Speed Evolution by Speed Controller")
plt.xlabel("Time (seconds)")
plt.ylabel("Speed (pulses per second)")
plt.ylim(0, 2800)
plt.legend()
plt.show()
