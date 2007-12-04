for type in clustering web_data web_pages
do
	for file in `find ./trial/$type -name '*.xml' | cut -d '/' -f 4 | sed 's/\([a-z._]*\).xml/\1/'` 
	do
		echo $file
		tidy -xml -latin1 -raw -c -n -i -q ./trial/$type/$file.xml | ./toutf8.pl > ./clean/$file.tidy.xml 
	done
done
