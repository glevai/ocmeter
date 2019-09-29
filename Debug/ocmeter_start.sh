cd /home/glevai/ocmeter/Debug
for ((;;)) do ./ocmeter -c config.yml -u 'http://192.168.0.163:8080/photoaf.jpg' -w -s10000 -vINFO;done
