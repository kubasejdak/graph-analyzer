from django.db import models

# PendingFile table
class PendingFile(models.Model):
    name = models.CharField(max_length = 100)