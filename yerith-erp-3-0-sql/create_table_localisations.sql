create table localisations (id int primary key, numero_unique varchar(256), adresse_ip varchar(256), dbms varchar(256), nom_localisation varchar(256) NOT NULL, description_lieu varchar(256), quartier varchar(256), ville varchar(256), province_etat varchar(256), pays varchar(256), boite_postale varchar(256), date_ouverture date, email varchar(256), numero_telephone_1 varchar(256), numero_telephone_2 varchar(256));