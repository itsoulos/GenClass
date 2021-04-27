import ctypes
import numpy as np
from typing import List

def classifier(input: List):
	fun = ctypes.CDLL("./classifier.so")
	fun.classifier.argtypes = [ctypes.POINTER(ctypes.c_double)]
	fun.classifier.restype = ctypes.c_int
	a = np.array(input)
	input_ptr = a.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
	return fun.classifier(input_ptr)


