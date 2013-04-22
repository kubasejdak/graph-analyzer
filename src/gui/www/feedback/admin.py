from django.contrib import admin
from models import Feedback

class FeedbackAdmin(admin.ModelAdmin):
    list_display = ("type", "description", "status")
    ordering = ("type", "status")

admin.site.register(Feedback, FeedbackAdmin)