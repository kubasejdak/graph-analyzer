from django.db import models

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
