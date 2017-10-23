static inline
void delay(unsigned int delta){
	//Note: The maximum allowed delta value here is 65534.
	//TODO: This function could be improved to compute msecs or seconds...
	//      Alternatively we could also use hardware interrupts
        //      to keep track of time with better precision.
	unsigned int t = 0;
	while (t<delta){
		t++;
	}
}
