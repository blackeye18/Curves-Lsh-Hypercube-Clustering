# Curves-Lsh-Hypercube-Clustering
DIT 2nd Project 2021-22

Georgios Georgiou - Γεώργιος Γεωργίου - sdi1800220 - 1115201800220

Panagiotis Mavrommatis - Παναγιώτης Μαυρομμάτης - sdi1800115 - 1115201800115

Τιτλος: Nearest neighour using LSH with curves and Clustering using kmeans++ with either Lloyds or LSH for curves.

Περιγραφή: Στο Α κομμάτι της εργασίας, βρίσκουμε τον κοντινότερο γείτονα, χρησιμοποιώντας είτε τον αλγόριθμο LSH είτε τον αλγόριθμο Hypercube για vectors είτε τον αλγόριθμο LSH Frechet για καμπύλες, είτε με continuous απόσταση είτε με discrete. Για να γίνει σύγκριση αποτελεσμάτων, υπολογίζουμε και σειριακά τους κοντινότερους γείτονες.
Στο Β κομμάτι της εργασίας κάνουμε clustering, παίρνοντας τα αρχικά centroids με την βοήθεια του αλγόριθμου kmeans++. Σε δεύτερη φάση, χρησιμοποιούμε είτε τον αλγόριθμο Lloyds, είτε κάνουμε radius search στο LSH ή στο Hypercube, για να υπολογίσουμε τα νέα, καλύτερα, centroids. Σε περίπτωση που χρησιμοποιούμε καμπύλες, παίρνουμε τα αρχικά centroids με την βοήθεια του αλγόριθμου kmeans++ και σε δεύτερη φάση χρησιμοποιούμε είτε τον αλγόριθμο Lloyds, είτε κάνουμε radius search στο LSH, χρησιμοποιώντας την discrete απόσταση Frechet. 
Επιπλέον με την χρήση της silhouette βλέπουμε κατά πόσο καλό είναι το clustering που κάναμε.
