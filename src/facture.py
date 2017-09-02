#!/usr/bin/python
#-*- coding: latin-1 -*-
import os, sys

def print_date():
	from time import *
	return strftime("%d %B %Y", localtime(time()))

def compte_choisi():
	comptes = []
	f = open("/home/jerzy/Rach/data/clients")
	line = f.readline()
	l = 0
	while line:
		l += 1
		print l, "-", line,
		comptes.append(line)
		line = f.readline()
	f.close()
	nr_compte = raw_input("Client à facturer: ")
	return comptes[int(nr_compte) - 1]

nr_facture = 0
f = open("/home/jerzy/Rach/data/facture_nr")
nr_facture = int(f.readline())
f.close()

f = open("/home/jerzy/Rach/data/facture_nr", "w")
if nr_facture < 988:
	print >>f, "%3d" %(nr_facture + 12),
else:	print >>f, "%3d" %(100),
f.close()

cheques = []
nb_cheques = 0
total = 0.00
tva = 0.00
o_n = "o"
while o_n == "o":
	cheque = []
	cheque.append(raw_input("Produit: "))
	cheque.append(raw_input("Quantité: "))
	cheque.append(raw_input("Prix unitaire: "))
	cheque.append(raw_input("Taux de TVA: "))
	cheques.append(cheque)
	nb_cheques += 1
	total += (float(cheque[1])*float(cheque[2]))
	tva += (float(cheque[1])*float(cheque[2])*float(cheque[3])/100)
	o_n = raw_input("Un autre produit o/n?: ")

form = """\


         MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA
	 MA                                                       MA
         MA                      Laurier Tomaka                   MA
	 MA	                                                  MA
	 MA Mas de l'Aiguillon        13520 Maussane les Alpilles MA
         MA                                                       MA
	 MA Tél.: 0490543041               E-mail: api@tomaka.org MA
	 MA                                                       MA
	 MA                Nr de TVA: FR60432114452               MA
	 MA		                                          MA
	 MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA


                     FACTURE Nr %(nr)d du %(date)17s
			      
			
         %(compte)s
	 
         -----------------------------------------------------------
         Produit                            Q  PU_HT  Montant  T_TVA
         -----------------------------------------------------------
"""
f = open("/home/jerzy/Rach/data/facture.txt", "w")
print >>f, form % {'nr':nr_facture, 'date':print_date(), 'compte':compte_choisi(),}

for cheque in cheques:
	print >>f, " "*8,
	print >>f, "%-29s  %5.0f  %5.2f %8.2f  %5.2f" %(cheque[0], float(cheque[1]),float(cheque[2]), (float(cheque[1])*float(cheque[2])), float(cheque[3]))

form = """\
         -----------------------------------------------------------
         -----------------------------------------------------------
         Total HT:    %(total)9.2f
         TVA:         %(tva)9.2f
         ----------------------
	 ----------------------

	 NET A PAYER: %(net)9.2f  euros
	 
"""
print >>f, form % {'total':total, 'tva':tva, 'net':total+tva,}
f.close()

from os import *
answer = 1
while int(answer) != 0:
	answer = raw_input("0 - Sortir du programme;\n"
			   "1 - Editer la facture;\n"
			   "2 - Imprimer la facture: ")
	if int(answer) == 1:
		system("vim /home/jerzy/Rach/data/facture.txt")
	elif int(answer) == 2:
		system("lp -n 2 /home/jerzy/Rach/data/facture.txt")
	else: break
