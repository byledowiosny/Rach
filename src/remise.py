#!/usr/bin/python

def print_date():
	from time import *
	return strftime("%d %B %Y", localtime(time()))

def compte_choisi():
	comptes = []
	f = open("/home/Rach/tools/compte")
	line = f.readline()
	l = 0
	while line:
		l += 1
		print l, "-", line,
		comptes.append(line)
		line = f.readline()
	f.close()
	nr_compte = raw_input("Compte de remise: ")
	return comptes[int(nr_compte) - 1]

cheques = []
nb_cheques = 0
total = 0.00
o_n = "o"
while o_n == "o":
	cheque = []
	cheque.append(raw_input("Emeteur: "))
	cheque.append(raw_input("Banque: "))
	cheque.append(raw_input("Montant: "))
	cheques.append(cheque)
	nb_cheques += 1
	total += float(cheque[2])
	o_n = raw_input("Un autre chèque o/n?: ")

form = """\


			Remise de chèques du %(date)s
		sur le compte: %(compte)s

             -------------------------------------------------------
             Emetteur       	Banque			     Montant
             -------------------------------------------------------
"""
f = open("/home/Rach/tools/remise.txt", "w")
print >>f, form % {'date':print_date(), 'compte':compte_choisi(),}

for cheque in cheques:
	print >>f, " "*12,
	print >>f, "%-18s %-26s %9.2f" % (cheque[0], cheque[1], float(cheque[2]))

form = """\
             -------------------------------------------------------
             -------------------------------------------------------
             Nb de chèques: %(nb_cheques)d
             -----------------------------
             Montant total: %(total)0.2f
"""
print >>f, form % {'nb_cheques':nb_cheques, 'total':total,}
f.close()

from os import *
answer = 1
while int(answer) != 0:
	answer = raw_input("0 - Sortir du programme;\n"
			   "1 - Editer la remise;\n"
			   "2 - Imprimer la remise: ")
	if int(answer) == 1:
		system("vim /home/Rach/tools/remise.txt")
	elif int(answer) == 2:
		system("lp -n 2 /home/Rach/tools/remise.txt")
	else: break
