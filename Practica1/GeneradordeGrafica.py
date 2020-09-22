import numpy as np
import matplotlib.pyplot as plt

datos=[]
datos1=[]
archivo=open("multiplicacion6blocks.txt","r")
archivo2=open("Clasica.txt","r")

datos=archivo.readlines()
datos1=archivo2.readlines()

archivo.close()
archivo2.close()
datos1=[w.strip() for w in  datos1]
datos1=[float(x) for x in datos1]
datos=[w.strip() for w in  datos]
datos=[float(x) for x in datos]

print(datos1)
print(datos)
tam=[100,400,700,1000,1300,1600,1900,2200,2500,2800]


plt.figure(figsize=(10,8))
plt.plot(tam,datos1,color='r')
plt.plot(tam,datos,color='g')
plt.xlabel("Tamaño de la matriz",fontsize=18)
plt.ylabel("tiempo de ejecucion",fontsize=18)
plt.legend(["Multplicacion Clasica","Multiplicacion por bloques"],loc='best')
plt.grid()
plt.show()
