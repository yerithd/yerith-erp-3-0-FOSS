create table groupes_de_paie_hr (id int primary key, date_creation date, reference_groupe_de_paie_hr varchar(256), designation varchar(256) NOT NULL UNIQUE, localisation varchar(256), description_groupes_de_paie_hr varchar(256) NOT NULL, MONTANT_A_PAYER_MENSUEL double NOT null, pourcentage_TAXES_IMPOSABLES double, intitule_de_la_ligne_budgetaire varchar(256) NOT NULL);