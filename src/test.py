#!/usr/bin/python

def print_date():
	from time import *
	return strftime("%d %B %Y", localtime(time()))

def compte_choisi():
	comptes = []
	f = open("/home/rach/tools/clients")
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
	 MA Tél.: 0490543041    E-mail: georges.tomaka@wanadoo.fr MA
	 MA                                                       MA
	 MA                Nr de TVA: FR60432114452               MA
	 MA		                                          MA
	 MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA*MA


                              Facture du %(date)s
			      
			
         %(compte)s
	 
         ------------------------------------------------------------
         Produit                            Q  PU_HT   Montant  T_TVA
         ------------------------------------------------------------
"""
f = open("/home/rach/tools/facture.txt", "w")
print >>f, form % {'date':print_date(), 'compte':compte_choisi(),}

for cheque in cheques:
	print >>f, " "*12,
	print >>f, "%-30s %4.0f %4.2f %7.2 %4.2"
	% (cheque[0], float(cheque[1]), float(cheque[2]),
	   float(cheque[1])*float(cheque[2]), float(cheque[3]))

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
			   "1 - Editer la facture;\n"
			   "2 - Imprimer la facture: ")
	if int(answer) == 1:
		system("vim /home/rach/tools/facture.txt")
	elif int(answer) == 2:
		system("lp -n 2 /home/rach/tools/facture.txt")
	else: break
