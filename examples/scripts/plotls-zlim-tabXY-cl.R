#1

cl = commandArgs()

filename = cl[6]

scale = 400

x <- seq(0:scale)
y <- seq(0:scale)
SS   <- read.table(filename,header=T)

z <- matrix(0, scale+1, scale+1)
nrows <- nrow(SS)
for (i in seq(1:nrows)) {
  if ( SS[i,1] <= scale && SS[i,2] <= scale)
  {
    z[SS[i,1]+1, SS[i,2]+1] <- SS[i,3]
  }
}


postscript(paste(filename,"-zlim.eps",sep=""), horizontal=F, width=8, height=5.5, onefile = TRUE, paper = "special", title = filename)
par(mar=c(1.5,2.2,0,0))

persp(x, y, z, theta = 130, phi = 20, expand = 0.5, col = "lightblue", ltheta = 150, shade = 0.8, ticktype = "detailed", xlab = "X", ylab = "Y", zlab = "Probability", border=NA, cex = 1.1, font = 2, zlim=c(-6e-6,5e-6)) -> res



dev.off()


