gcc pbkdf2.c -o pbkdf2 -lssl -lcrypto
./pbkdf2
#openssl des-ecb -provider legacy -provider default -pbkdf2 -P -S 123456789ABCDEF0
#openssl des-ecb -provider legacy -provider default -pbkdf2 -P -nosalt
#openssl des-ecb -provider legacy -provider default -pbkdf2 -P -S 123456789ABCDEF0 -iter 1000 
openssl des-ecb -provider legacy -provider default -pbkdf2 -P -S 123456789ABCDEF0 -iter 10000 -pass pass:mypassword


#cat Makefile | openssl des-ecb -provider legacy -provider default -k a091987844e55af1 | openssl des-ecb -provider legacy -provider default -d -k a091987844e55af1
#cat Makefile | ./ft_ssl des-ecb -k a091987844e55af1 | openssl des-ecb -provider legacy -provider default -d -K a091987844e55af1