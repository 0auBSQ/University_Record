all:
	find * -name "*.java" > sources.txt
	javac -sourcepath . @sources.txt

clean:
	find * -name "*.class" -delete
	rm sources.txt
