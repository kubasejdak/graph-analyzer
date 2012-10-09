from django import forms
from options.models import Option

class OptionsForm(forms.Form):
    output_dest = forms.ChoiceField(choices = Option.OUTPUT_CHOICES)
    log_level = forms.ChoiceField(choices = Option.LOG_CHOICES)
    log_file = forms.CharField()