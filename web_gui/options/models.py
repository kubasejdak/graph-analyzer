from django.db import models

# SystemInfo table
class SystemInfo(models.Model):
    version = models.CharField(max_length = 10)
    status = models.CharField(max_length = 50)
    error = models.CharField(max_length = 50)
    progress = models.IntegerField()
    exploits_num = models.IntegerField()
    samples_num = models.IntegerField()

# Options table
class Option(models.Model):
    OUTPUT_CHOICES = (
        ("DatabaseOutput", "DatabaseOutput"),
        ("ConsoleOutput", "ConsoleOutput"),
    )
    
    LOG_CHOICES = (
        (1, "Low"),
        (2, "Normal"),
        (3, "High"),
    )
    
    output_dest = models.CharField(max_length = 20, choices = OUTPUT_CHOICES)
    log_level = models.IntegerField(choices = LOG_CHOICES)
    log_level = models.IntegerField()
    log_file = models.CharField(max_length = 100)

# PendingFile table
class PendingFile(models.Model):
    name = models.CharField(max_length = 100)