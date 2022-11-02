
cl = commandArgs()

filename = cl[6]



ucurve<-read.table(filename,header=T)


postscript(paste(filename,".eps",sep=""), horizontal=F, width=8, height=5.5, onefile = TRUE, paper = "special")
plot(ucurve$X,ucurve$Prob,pch=16, xlab="S",ylab="Probability", cex.lab=1.5, cex.axis=1.5, cex=1.5, cex.main=2 , type="l", col=4, lwd=2, lty=1, xlim=c(0,150))

# seperating lines
#lines(X, Y, pch=3, type="l", cex=1.5, col="red", lwd=1, lty=1)

dev.off()

