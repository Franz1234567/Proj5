import re
import matplotlib.pyplot as plt
import numpy as np

# Fonction de traitement pour récupérer les vitesses
def extract_speeds(file_path):
    reference_speeds = []
    measured_speeds = []

    with open(file_path, 'r') as file:
        for line in file:
            # Vérifier que la ligne commence par "Reference"
            if line.startswith("Reference"):
                # Extraire les valeurs avec une expression régulière
                match = re.search(r'Reference speed: (\d+).*?Measured speed: (\d+)', line)
                if match:
                    # Ajouter les vitesses extraites aux listes
                    reference_speeds.append(int(match.group(1)))
                    measured_speeds.append(int(match.group(2)))
    
    return reference_speeds, measured_speeds

file_path = 'test.txt'  # Remplacez par le chemin de votre fichier
reference_speeds, measured_speeds = extract_speeds(file_path)

print("Reference Speeds:", reference_speeds)
print("Measured Speeds:",measured_speeds)

time_points = np.arange(0, len(reference_speeds) * 0.25, 0.25)

# Création du graphique
plt.figure(figsize=(10, 5))
plt.plot(time_points, reference_speeds, color='blue', label='Reference Speed')
plt.plot(time_points, measured_speeds, color='red', label='Measured Speed')
plt.xlabel('Time in seconds')
plt.ylabel('Speed in pps')
plt.xlim(0, 60)
plt.ylim(0, 2500)

# Ligne optionnelle pour 63% de la vitesse de référence initiale
# plt.axhline(y=0.63 * reference_speeds[0], color='green', linestyle='--', label='63% of Initial Reference Speed')

plt.title('Reference Speed vs Measured Speed')
plt.legend()
plt.grid(True)
plt.show()
