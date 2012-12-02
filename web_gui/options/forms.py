from django import forms
from options.models import Option

class OptionsForm(forms.Form):
    log_level = forms.ChoiceField(choices = Option.LOG_CHOICES)
    log_file = forms.CharField()
    
    resemblence_level = forms.ChoiceField(choices = Option.RESEMBLENCE_CHOICES)
    emulation_steps = forms.IntegerField()
    graphs_dir = forms.CharField()
    big_files_protection = forms.BooleanField()
    max_input_file_size = forms.IntegerField()
    skip_empty_samples = forms.BooleanField()
    skip_broken_samples = forms.BooleanField()
    broken_samples_size = forms.IntegerField()