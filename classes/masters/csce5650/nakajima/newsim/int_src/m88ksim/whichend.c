int main(){
#ifdef LEHOST
	return 'l';
#else
	return 'b';
#endif
}
