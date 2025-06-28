import bpy
import os
import mathutils # For matrix and vector operations

# --- Helper Functions (adapted for bpy) ---
# vec_add is generally not needed as Blender's mathutils handles vector addition directly
# def vec_add(v1, v2, v3):
#     v3[0] = v1[0] + v2[0]
#     v3[1] = v1[1] + v2[1]
#     v3[2] = v1[2] + v2[2]

# vec_transform can be replaced with mathutils.Vector * mathutils.Matrix
# def vec_transform(p, m):
#     return [p[0] * m[0][0] + p[1] * m[1][0] + p[2] * m[2][0] + m[3][0],
#             p[0] * m[0][1] + p[1] * m[1][1] + p[2] * m[2][1] + m[3][1],
#             p[0] * m[0][2] + p[1] * m[1][2] + p[2] * m[2][2] + m[3][2]]

# Get all materials in the scene
mats = bpy.data.materials
export_mats = len(mats) > 0

# --- Material Export ---
if export_mats:
    try:
        with open("mtl.xml", "w") as file:
            file.write("<?xml version=\"1.0\"?>\n\n")
            file.write("\n")

            file.write("<material-lib name=\"mtl\" version=\"1.0\">\n")
            file.write("\t<material-group name=\"mtl\" default=\"1\">\n")

            for mat in mats:
                # Default values for material properties
                current_alpha = 1.0 # Assume opaque by default
                current_emit = 0.0  # Assume no emission by default
                
                # These are fallback defaults if properties aren't found on the material or principled node
                rgb = (0.8, 0.8, 0.8) # Default diffuse
                spec = (0.04, 0.04, 0.04) # Default specular
                shine = 50.0 # Default shininess

                mtextures = []

                # --- Prioritize Node-based Material Properties ---
                if mat.use_nodes:
                    principled_node = None
                    for node in mat.node_tree.nodes:
                        if node.type == 'BSDF_PRINCIPLED':
                            principled_node = node
                            break

                    if principled_node:
                        # Get alpha from Principled BSDF's Alpha input
                        alpha_socket = principled_node.inputs.get('Alpha')
                        if alpha_socket:
                            if not alpha_socket.links: # If Alpha input is not linked, use its default value
                                current_alpha = alpha_socket.default_value

                        # Get emission from Principled BSDF's Emission Color/Strength
                        emission_strength_socket = principled_node.inputs.get('Emission Strength')
                        if emission_strength_socket and not emission_strength_socket.links:
                            current_emit = emission_strength_socket.default_value

                        # Get base color (diffuse) and associated image texture
                        base_color_socket = principled_node.inputs.get('Base Color')
                        if base_color_socket:
                            if not base_color_socket.links:
                                rgb = base_color_socket.default_value[:3] # RGB from RGBA
                            else: # If base color is linked, try to find an image texture
                                for link in base_color_socket.links:
                                    if link.from_node.type == 'TEX_IMAGE' and link.from_node.image:
                                        mtextures.append({'name': link.from_node.name, 'image': link.from_node.image})
                                        break # Assuming only one base color texture

                        # Shininess from roughness (PBR model)
                        roughness_socket = principled_node.inputs.get('Roughness')
                        if roughness_socket and not roughness_socket.links:
                            shine = (1.0 - roughness_socket.default_value) * 100 # Invert roughness for shininess and scale
                        
                        # Specular color directly from Principled BSDF's Specular Color input if available and not linked
                        # Note: 'Specular Color' socket is relatively new, older Principled BSDF might not have it.
                        # It's more common to get a "tint" or a 'Specular' factor.
                        # We'll prioritize the material's top-level specular_color for simplicity.
                        # If a direct RGB specular input existed and was flat, you'd add:
                        # specular_color_socket = principled_node.inputs.get('Specular Color')
                        # if specular_color_socket and not specular_color_socket.links:
                        #     spec = specular_color_socket.default_value[:]


                # --- Fallback for Material Properties (for non-node materials or if not found via Principled node) ---
                # These attributes are often present even if use_nodes is False,
                # or act as a primary source if no principled node is found/used.
                if hasattr(mat, 'diffuse_color'):
                    rgb = mat.diffuse_color[:3]
                if hasattr(mat, 'specular_color'):
                    spec = mat.specular_color[:]
                if hasattr(mat, 'roughness'):
                    shine = (1.0 - mat.roughness) * 100
                if hasattr(mat, 'alpha'): # This 'alpha' is sometimes present for non-node transparency settings
                    current_alpha = mat.alpha
                if hasattr(mat, 'emit'):
                    current_emit = mat.emit


                # Adjust alpha based on material's blend method (overrides other alpha if needed)
                # This is important for indicating overall material transparency.
                if mat.blend_method != 'OPAQUE' and current_alpha == 1.0:
                    # If the blend method indicates transparency, but our alpha is still 1.0,
                    # set a default transparent value to signal transparency in XML.
                    current_alpha = 0.5 # A default for transparency, adjust as needed

                file.write("\t\t<material name=\"%s\">\n" % (mat.name))
                file.write("\t\t\t<color name=\"ambient\"  r=\"%s\" g=\"%s\" b=\"%s\" />\n" % (rgb[0], rgb[1], rgb[2]))
                file.write("\t\t\t<color name=\"diffuse\"  r=\"%s\" g=\"%s\" b=\"%s\" a=\"%s\" />\n" % (rgb[0], rgb[1], rgb[2], current_alpha))
                file.write("\t\t\t<color name=\"specular\" r=\"%s\" g=\"%s\" b=\"%s\" />\n" % (spec[0], spec[1], spec[2]))
                file.write("\t\t\t<color name=\"emissive\" r=\"%s\" g=\"%s\" b=\"%s\" />\n" % (current_emit, current_emit, current_emit))
                file.write("\t\t\t<shininess value=\"%s\" />\n" % (shine))

                for mtex_info in mtextures:
                    img_name = mtex_info['name']
                    img = mtex_info['image']
                    if img and img.filepath:
                        filename = os.path.basename(bpy.path.abspath(img.filepath))
                        file.write("\t\t\t<texture name=\"%s\" file=\"%s\" />\n" % (img_name, filename))
                
                file.write("\t\t</material>\n")
            file.write("\t</material-group>\n")
            file.write("</material-lib>\n")

    except IOError as e:
        print(f"Error writing mtl.xml: {e}")

# --- Mesh Export ---
try:
    with open("obj.xml", "w") as file:
        file.write("<?xml version=\"1.0\"?>\n\n")
        file.write("\n")

        file.write("<mesh name=\"%s\" version=\"1.0\">\n" % ("mesh"))

        if export_mats:
            file.write("\t<material-lib name=\"mtl.xml\" />\n")

    # ###############################################################################
    ## WRITE VERTICES
    # ###############################################################################
        file.write("\t<vertices>\n")

        i = 0
        # Iterate over all mesh objects in the current scene
        objs = [obj for obj in bpy.context.scene.objects if obj.type == 'MESH']
        for obj in objs:
            # Ensure the object's mesh data is evaluated for modifiers
            dg = bpy.context.evaluated_depsgraph_get()
            eval_mesh = obj.evaluated_get(dg).data
            
            verts = eval_mesh.vertices
            matrix = obj.matrix_world # Use world matrix for global coordinates

            file.write("\t\t<!-- %s -->\n" % (obj.name))
            for v in verts:
                # Apply object's world matrix to vertex coordinate
                co = matrix @ v.co

                file.write("\t\t<vertex id=\"%d\" x=\"%s\" y=\"%s\" z=\"%s\" />\n" % (i, co.x, co.y, co.z))

                i += 1
            # Free the evaluated mesh data to prevent memory leaks
            eval_mesh.user_clear()

        file.write("\t</vertices>\n")

    # ###############################################################################
    ## WRITE TEXTURE COORDS
    # ###############################################################################
        file.write("\t<texture-coords>\n")

        i = 0
        objs = [obj for obj in bpy.context.scene.objects if obj.type == 'MESH']
        for obj in objs:
            dg = bpy.context.evaluated_depsgraph_get()
            eval_mesh = obj.evaluated_get(dg).data

            # Check for UV layers
            if not eval_mesh.uv_layers:
                eval_mesh.user_clear()
                continue # No UVs, skip this object's UV export

            # Get the active UV layer
            uv_layer = eval_mesh.uv_layers.active.data
            
            file.write("\t\t<!-- %s -->\n" % (obj.name))
            
            # UV coordinates are per loop (vertex-face corner), not per face
            # We need to iterate over mesh loops to get UVs
            for poly in eval_mesh.polygons:
                for loop_index in poly.loop_indices:
                    uv = uv_layer[loop_index].uv
                    file.write("\t\t<uv id=\"%s\" u=\"%s\" v=\"%s\" />\n" % (i, uv.x, uv.y))
                    i += 1
            eval_mesh.user_clear()

        file.write("\t</texture-coords>\n")

    # ###############################################################################
    ## WRITE VERTEX GROUPS (Commented out as in original, but updated for bpy)
    # ###############################################################################
        # file.write("\t<vertex-groups>\n")
        #
        # v_offset = 0 # Offset for vertex IDs across multiple objects
        # objs = [obj for obj in bpy.context.scene.objects if obj.type == 'MESH']
        # for obj in objs:
        #     dg = bpy.context.evaluated_depsgraph_get()
        #     eval_mesh = obj.evaluated_get(dg).data
        #
        #     # Vertex groups are accessed through obj.vertex_groups
        #     for group in obj.vertex_groups:
        #         file.write("\t\t<vertex-group name=\"%s::%s\">\n" % (eval_mesh.name, group.name))
        #         for v in eval_mesh.vertices:
        #             for vg in v.groups:
        #                 if vg.group == group.index:
        #                     file.write("\t\t\t<vertex v=\"%s\" weight=\"%s\" />\n" % (v.index + v_offset, vg.weight))
        #         file.write("\t\t</vertex-group>\n")
        #     v_offset += len(eval_mesh.vertices)
        #     eval_mesh.user_clear()
        # file.write("\t</vertex-groups>\n")

    # ###############################################################################
    ## WRITE NORMALS
    # ###############################################################################
        file.write("\t<facet-normals>\n")

        i = 0
        objs = [obj for obj in bpy.context.scene.objects if obj.type == 'MESH']
        for obj in objs:
            dg = bpy.context.evaluated_depsgraph_get()
            eval_mesh = obj.evaluated_get(dg).data

            file.write("\t\t<!-- %s -->\n" % (obj.name))
            for f in eval_mesh.polygons:
                # Polygon normals are directly available
                file.write("\t\t<normal id=\"%s\" x=\"%s\" y=\"%s\" z=\"%s\" />\n" % (i, f.normal.x, f.normal.y, f.normal.z))
                i += 1
            eval_mesh.user_clear()

        file.write("\t</facet-normals>\n")


        file.write("\t<vertex-normals>\n")

        i = 0
        objs = [obj for obj in bpy.context.scene.objects if obj.type == 'MESH']
        for obj in objs:
            dg = bpy.context.evaluated_depsgraph_get()
            eval_mesh = obj.evaluated_get(dg).data

            file.write("\t\t<!-- %s -->\n" % (obj.name))
            for v in eval_mesh.vertices:
                # Vertex normals are directly available
                file.write("\t\t<normal id=\"%s\" x=\"%s\" y=\"%s\" z=\"%s\" />\n" % (i, v.normal.x, v.normal.y, v.normal.z))
                i += 1
            eval_mesh.user_clear()

        file.write("\t</vertex-normals>\n")

    # ###############################################################################
    ## WRITE OBJECTS
    # ###############################################################################
        file.write("\t<objects>\n")

        face_offset = 0
        vertex_offset = 0
        uv_offset = 0
        
        objs = [obj for obj in bpy.context.scene.objects if obj.type == 'MESH']
        for obj in objs:
            dg = bpy.context.evaluated_depsgraph_get()
            eval_mesh = obj.evaluated_get(dg).data
            
            has_uv = bool(eval_mesh.uv_layers)
            
            file.write("\t\t<object name=\"%s\">\n" % (obj.name))

            for f_idx, poly in enumerate(eval_mesh.polygons):
                # The original script exports "edges" (polygons with 2 vertices or less) by skipping them.
                # In modern Blender, polygons are always >= 3 vertices.
                
                mat_name = ""
                # Ensure the material index is valid and the material slot exists
                if export_mats and poly.material_index < len(eval_mesh.materials) and eval_mesh.materials[poly.material_index]:
                    mat_name = f"material=\"{eval_mesh.materials[poly.material_index].name}\""

                vertex_norms_smooth = poly.use_smooth # True if smoothed

                if not vertex_norms_smooth:
                    # If not smooth, use face normal. Original script used a global `i` for this.
                    # This maps to a unique normal ID for each face.
                    norm_id = face_offset + f_idx 
                    file.write("\t\t\t<face %s normal=\"%s\">\n" % (mat_name, norm_id))
                else:
                    file.write("\t\t\t<face %s>\n" % (mat_name))

                
                for loop_idx, vertex_idx in enumerate(poly.vertices):
                    norm_attr = ""
                    if vertex_norms_smooth:
                        # Original script used v.index+voff for vertex normal ID.
                        # This maps directly to vertex index + current object's vertex offset.
                        norm_attr = f"vn=\"{vertex_idx + vertex_offset}\""

                    if not has_uv:
                        file.write("\t\t\t\t<vertex v=\"%s\" %s />\n" % (vertex_idx + vertex_offset, norm_attr))
                    else:
                        file.write("\t\t\t\t<vertex v=\"%s\" %s>\n" % (vertex_idx + vertex_offset, norm_attr))
                        # The UV ID is based on the global UV loop index
                        file.write("\t\t\t\t\t<vt uv=\"%s\" />\n" % (poly.loop_indices[loop_idx] + uv_offset))
                        file.write("\t\t\t\t</vertex>\n")

                file.write("\t\t\t</face>\n")
            
            face_offset += len(eval_mesh.polygons)
            vertex_offset += len(eval_mesh.vertices)
            # Accumulate UV offset based on total loops in active UV layer
            uv_offset += len(eval_mesh.uv_layers.active.data) if eval_mesh.uv_layers else 0 
            eval_mesh.user_clear() # Free the evaluated mesh data

        file.write("\t</objects>\n")

    # ###############################################################################
    ## WRITE ARMATURES
    # ###############################################################################
        file.write("\t<armatures>\n")

        # In modern Blender, armatures are in bpy.data.armatures
        armatures = bpy.data.armatures
        for arm_data in armatures:
            file.write("\t\t<armature name=\"%s\">\n" % (arm_data.name))
            # Bones are accessed via arm_data.bones
            for bone in arm_data.bones:
                file.write("\t\t\t<bone name=\"%s\" />\n" % (bone.name))

            file.write("\t\t</armature>\n")

        file.write("\t</armatures>\n")


        file.write("</mesh>\n")

except IOError as e:
    print(f"Error writing obj.xml: {e}")

print("Export complete!")