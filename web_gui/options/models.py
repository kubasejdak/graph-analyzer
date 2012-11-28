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
    
    RESEMBLENCE_CHOICES = (
        (0, "0 %"),
        (10, "10 %"),
        (20, "20 %"),
        (30, "30 %"),
        (40, "40 %"),
        (50, "50 %"),
        (60, "60 %"),
        (70, "70 %"),
        (80, "80 %"),
        (90, "90 %"),
        (100, "100 %"),
    )
    
    output_dest = models.CharField(max_length = 20, choices = OUTPUT_CHOICES)
    log_level = models.IntegerField(choices = LOG_CHOICES)
    log_level = models.IntegerField()
    log_file = models.CharField(max_length = 100)
    resemblence_level = models.IntegerField(choices = RESEMBLENCE_CHOICES)

# PendingFile table
class PendingFile(models.Model):
    name = models.CharField(max_length = 100)

# RecentFile table
class RecentFile(models.Model):
    name = models.CharField(max_length = 100)

# Feedback table
class Feedback(models.Model):
    TYPE_CHOICES = (
        ("bug", "bug"),
        ("feature", "feature"),
    )

    STATUS_CHOICES = (
        ("pending", "pending"),
        ("resolved", "resolved"),
    )

    type = models.CharField(max_length = 20, choices = TYPE_CHOICES)
    description = models.CharField(max_length = 3000)
    status = models.CharField(max_length = 20, choices = STATUS_CHOICES)