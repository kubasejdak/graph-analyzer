from django import forms

class OptionsForm(forms.Form):
    LOG_CHOICES = (
        (0, "OFF"),           
        (1, "Low"),
        (2, "Normal"),
        (3, "High"),
    )
    
    log_level = forms.ChoiceField(choices = LOG_CHOICES)
    log_file = forms.CharField()