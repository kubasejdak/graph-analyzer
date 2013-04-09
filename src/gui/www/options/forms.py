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
    
    emulation_steps = forms.IntegerField()
    graphs_dir = forms.CharField()
    big_files_protection = forms.BooleanField(required = False)
    max_input_file_size = forms.IntegerField()
    skip_empty_samples = forms.BooleanField(required = False)
    skip_broken_samples = forms.BooleanField(required = False)
    broken_samples_size = forms.IntegerField()