test:
	gcc midterm-example.c -lpthread -DTEST && ./a.out
submit:
	curl http://jyywiki.cn/upload -F course=OS2022 -F module=Midterm \
	 -F token=P9lRJZ9h -F file=@/home/hydra24/mid-test/midterm-example.c
