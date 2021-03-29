##Have to get back to this one later, couldn't get it to work. Used peanut in interactive mode.
#!/bin/bash
#
#SBATCH --mail-user=rohithj@cs.uchicago.edu
#SBATCH --mail-type=ALL
#SBATCH --chdir=/home/rohithj/slurm
#SBATCH --output=/home/rohithj/slurm/out/%j.%N.stdout
#SBATCH --error=/home/rohithj/slurm/out/%j.%N.stderr
#SBATCH --partition=fast
#SBATCH --job-name=raytracing
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --mem-per-cpu=500
#SBATCH --time=15:00

cmake ..
make raytracing
./raytracing <${1}
