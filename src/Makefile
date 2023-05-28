PATH_CAT = ./cat/
PATH_GREP = ./grep/

all: clean s21_cat s21_grep test style


build: s21_cat s21_grep

s21_cat:
	$(MAKE) -C $(PATH_CAT) s21_cat

s21_grep:
	$(MAKE) -C $(PATH_GREP) s21_grep


style: style_cat style_grep

style_cat:
	$(MAKE) -C $(PATH_CAT) style

style_grep:
	$(MAKE) -C $(PATH_GREP) style


test: test_cat test_grep

test_cat:
	$(MAKE) -C $(PATH_CAT) test

test_grep:
	$(MAKE) -C $(PATH_GREP) test


clean: clean_cat clean_grep

clean_cat:
	$(MAKE) -C $(PATH_CAT) clean

clean_grep:
	$(MAKE) -C $(PATH_GREP) clean


rebuild: clean build
