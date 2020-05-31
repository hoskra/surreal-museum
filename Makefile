doc:
	doxygen doxygen.config "$@"> /dev/null 2>&1
all:
	doc
clean:
	rm -rf doc/
clean-all: clean
	rm -rf Debug/ .vs .vscode
