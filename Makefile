all: man/manual-page.1 man/manual-page.html

man/manual-page.1: man/manual-page.markdown
	md2man-roff man/manual-page.markdown > man/manual-page.1

man/manual-page.html: man/manual-page.markdown
	md2man-html man/manual-page.markdown > man/manual-page.tmp.html && cat man/manual-page-template-1.html man/manual-page.tmp.html man/manual-page-template-2.html > man/manual-page.html

clean:
	rm man/manual-page.1 man/manual-page.html
