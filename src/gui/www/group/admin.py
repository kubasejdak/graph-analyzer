from django.contrib import admin
from models import Group, GroupAssignment

class GroupAdmin(admin.ModelAdmin):
    list_display = ("name", "leader", "members_num", "task", "comment")
    search_fields = ("name", "leader", "task", "comment", "members_num")
    list_filter = ("name", "leader", "task", "members_num")
    ordering = ("-members_num", "name", "leader")

class GroupAssignmentAdmin(admin.ModelAdmin):
    list_display = ("group", "member", "resemblence")
    search_fields = ("group", "member", "resemblence")
    list_filter = ("group", "member", "resemblence")
    ordering = ("-resemblence", "-group")

admin.site.register(Group, GroupAdmin)
admin.site.register(GroupAssignment, GroupAssignmentAdmin)