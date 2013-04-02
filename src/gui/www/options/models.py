from django.db import models

# SystemInfo table
class SystemInfo(models.Model):
    version = models.CharField(max_length = 10)
    status = models.CharField(max_length = 100)
    progress = models.IntegerField()
    error = models.CharField(max_length = 200)
    errors_num = models.IntegerField()

# Options table
class Option(models.Model):
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
    
    log_level = models.IntegerField(choices = LOG_CHOICES)
    log_file = models.CharField(max_length = 100)

    resemblence_level = models.IntegerField(choices = RESEMBLENCE_CHOICES)
    emulation_steps = models.IntegerField()
    graphs_dir = models.CharField(max_length = 100)
    big_files_protection = models.BooleanField()
    max_input_file_size = models.IntegerField()
    skip_empty_samples = models.BooleanField()
    skip_broken_samples = models.BooleanField()
    broken_samples_size = models.IntegerField()

# PendingFile table
class PendingFile(models.Model):
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